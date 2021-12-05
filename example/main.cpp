#include "../include/benchmark.h"
#include <iostream>
#include <vector>

using namespace corsac;

int main()
{
	auto b = BenchmarkManager(5);

	b.create_bench("Vector push", [=]()mutable{

		std::vector<int> c;
		
		for(int i = 0; i < 100000; i++)
			c.push_back(i);	
		
		return 0;
	});

	std::vector<int> c;
	for(int i = 0; i < 100000; i++)
			c.push_back(i);	

	b.create_bench("Vector ForEach", [=]()mutable{
		for(int i = 0; i < c.size(); i++)
			c[i] =+ 1;	
		
		return 0;
	});

	b.start_benchmarks();
	
	b.print_result();

	return 0;
}

