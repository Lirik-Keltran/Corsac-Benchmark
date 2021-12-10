#ifndef CORSAC_BENCHMARK
#define CORSAC_BENCHMARK
#include <functional>
#include <iostream>
#include <chrono>
#include <ctime>
#include <numeric>
#include <fstream>

namespace corsac{
	class BenchmarkManager;

	class Bench
	{
		private:
			std::string name;
			std::vector<unsigned int> amount_operations;
			std::vector<unsigned int> times;
			std::string date;

		public: 
			std::function<int()> bench;
			
			Bench(std::string name, std::function<int()> bench)
			{
				this->bench = bench;
				this->name = name;
			}

			std::string get_name()
			{
				return this->name;
			}

			std::string get_date()
			{
				return this->date;
			}

			void add_operation(unsigned int amount_operation)
			{
				this->amount_operations.push_back(amount_operation);
			}

			unsigned int get_operation(int index)
			{
				return this->amount_operations[index];
			}

			unsigned int get_max_operation()
			{
				return this->amount_operations.back();	
			}

			unsigned int get_median_operation()
			{
				return this->amount_operations[this->amount_operations.size()/2];	
			}

			unsigned int get_average_operation()
			{
				return (accumulate(	this->amount_operations.begin(), 
									this->amount_operations.end(), 
									0)/this->amount_operations.size());
			}

			void add_time(unsigned int time)
			{
				this->times.push_back(time);
			}


			float get_time(int index)
			{
				return static_cast<float>(this->times[index])/1000000;
			}

			float get_max_time()
			{
				return static_cast<float>(this->times.back())/1000000;	
			}

			float get_median_time()
			{
				return static_cast<float>(this->times[this->times.size()/2])/1000000;	
			}

			float get_average_time()
			{
				return static_cast<float>(accumulate(	this->times.begin(), 
									this->times.end(), 
									0)/this->times.size())/1000000;
			}

			void start_bench(int amount)
			{
				std::time_t date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());


				this->date = std::ctime(&date);

				for(unsigned int c = 0; c < amount; c++)
				{
					std::cout << "\r " << this->name << " time test " << c << "/" << amount;
					auto start = std::chrono::steady_clock::now();
					this->bench();
					auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start);						

					this->add_time(elapsed.count());
				}

			
				unsigned int amount_operation = 0;

				for(unsigned int c = 0; c < amount; c++)
				{
					std::cout << "\r " << this->name << " operations test " << c+1 << "/" << amount;
					auto start = std::chrono::steady_clock::now();
					for(auto end = std::chrono::steady_clock::now();
						std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() < 1000;
						end = std::chrono::steady_clock::now())
					{
						this->bench();
						amount_operation++;
					}
					this->amount_operations.push_back(amount_operation);

				}

				std::sort(
							this->amount_operations.begin(),
							this->amount_operations.end()
						);

				std::sort(
						this->times.begin(),
						this->times.end()
						);

				std::cout << "\r                                               \r";
			}
	};


	class BenchmarkManager
	{
		private:
			std::vector<Bench> benchs;
			unsigned int amount = 100;
			const std::string name_file_result = "Benchmark results.csv";
		public:
			BenchmarkManager(){}

			BenchmarkManager(unsigned int amount)
			{
				this->amount = amount;
			}

			void set_amount(int amount)
			{
				this->amount = amount;
			}

			void create_bench(std::string name, std::function<int()> bench)
			{
				this->benchs.push_back(Bench(name, bench));
			}

			void start_benchmarks()
			{
				int amount_bench = static_cast<float>(this->benchs.size());
				for(int i = 0; i < this->benchs.size(); i++)
				{
					this->benchs[i].start_bench(this->amount);
				}
			}

			void save_result()
			{
				std::string header = "Amount;Name;Min Time ms;Max Time ms;Avarge Time ms;Median Time ms;Min Operation op/s;Max Operation op/s;Avarge Operation op/s;Median op/s;Time";
			
				std::string file_header;

				std::ifstream csv_read(this->name_file_result);

				if (csv_read.is_open())
				{
					getline(csv_read, file_header);
					csv_read.close();
				}


				std::ofstream csv_write(this->name_file_result, std::ios::app);
				
				if(!csv_write.is_open())
				{
					std::cout << "error write result"<< std::endl;
				}

				if(file_header != header)
				{
					csv_write << header << "\n";	
				}




				for(int i = 0; i < this->benchs.size(); i++)
				{
					csv_write 	<< this->amount << ";";

					csv_write 	<< this->benchs[i].get_name() << ";";

					csv_write 	<< this->benchs[i].get_time(0) << ";";

					csv_write 	<< this->benchs[i].get_max_time() << ";";

					csv_write 	<< this->benchs[i].get_average_time() << ";";

					csv_write 	<< this->benchs[i].get_median_time()  << ";";


					csv_write 	<< this->benchs[i].get_operation(0)  << ";";
					
					csv_write 	<< this->benchs[i].get_max_operation()  << ";";

					csv_write 	<< this->benchs[i].get_average_operation() << ";";

					csv_write 	<< this->benchs[i].get_median_operation() << ";";
					
					csv_write 	<< this->benchs[i].get_date();
				}
				csv_write  << "\n";
				csv_write.close();
			}

			void print_result()
			{
				for(int i = 0; i < this->benchs.size(); i++)
				{
					std::cout << this->benchs[i].get_name() << ": " << std::endl;
					
					std::cout << "   Time:" << std::endl;
					std::cout 	<< "      Min: " 
								<< this->benchs[i].get_time(0) 
								<< "ms"
								<< std::endl;

					std::cout 	<< "      Max: " 
								<< this->benchs[i].get_max_time()
								<< "ms"
								<< std::endl;

					std::cout 	<< "      Average: " 
								<< this->benchs[i].get_average_time() 
								<< "ms"
								<< std::endl;

					std::cout 	<< "      Median: " 
								<< this->benchs[i].get_median_time() 
								<< "ms"
								<< std::endl;



					std::cout << "   Operations:" << std::endl;
					std::cout 	<< "      Min: " 
								<< this->benchs[i].get_operation(0) 
								<< "op/s"
								<< std::endl;
					
					std::cout 	<< "      Max: " 
								<< this->benchs[i].get_max_operation() 
								<< "op/s"
								<< std::endl;

					std::cout 	<< "      Average: " 
								<< this->benchs[i].get_average_operation()
								<< "op/s"
								<< std::endl;

					std::cout 	<< "      Median: " 
								<< this->benchs[i].get_median_operation()
								<< "op/s"
								<< std::endl;

					

				}
			}
	};

	
}

#endif