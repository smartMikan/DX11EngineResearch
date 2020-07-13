#pragma once
#include<stdint.h>
#include <random>

namespace Utility 
{
	std::random_device random_device;
	std::mt19937 seed_generator(random_device());

	inline int GetRandom(int min, int max) 
	{
		std::uniform_int_distribution<int> distr(min, max);
		return distr(seed_generator);
	}

	inline int GetRnadom(int max)
	{
		return GetRandom(0, max);
	}

	
}
