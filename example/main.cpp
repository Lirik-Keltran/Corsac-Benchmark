#include "../include/benchmark.h"
#include <iostream>
#include <vector>

using namespace corsac;

struct pos
{
	int x, y, z;
	pos(int x, int y, int z):x(x),y(y),z(z){}
};

int main()
{
	auto b = BenchmarkManager(25);

	std::vector<pos> c;
	for(int i = 0; i < 100000; i++)
			c.push_back(pos(0,0,0));	

	b.create_bench("Vector for", [=]()mutable{
		int sum = 0;
		for(int i = 0; i < c.size(); i++)
		{
			sum += c[i].x+c[i].y+c[i].z;	
		}
		
		return sum;
	});

	b.create_bench("Vector ForEach", [=]()mutable{
		int sum = 0;
		for(auto i = c.begin(), end = c.end(); i != end; ++i)
		{
			sum+=i->x+i->y+i->z;
		}
		return sum;
	});

	b.create_bench("Vector For in", [=]()mutable{
		int sum = 0;
		for(auto i: c)
		{
			sum += i.z+i.y+i.x;	
		}

		return sum;
	});

	b.create_bench("Vector std::for_each", [=]()mutable{
		int sum = 0;
		std::for_each(
			c.begin(), 
			c.end(), 
			[=](pos i)mutable 
			{ 
				sum += i.z+i.y+i.x;
			});

		return sum;
	});

	b.start_benchmarks();
	
	b.print_result();
	b.save_result();

	return 0;
}

