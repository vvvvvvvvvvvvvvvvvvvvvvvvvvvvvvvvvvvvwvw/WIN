#include "pch.h"

module Random;

namespace win
{
	int Random::Next(int min, int max)
	{
		uint32_t range = max - min;
		return	min + (int_h->Hash(win::system::PerformanceCounter::GetValue().QuadPart) % range);
	}
	double Random::NextDouble(double min, double max)
	{
		double range = max - min;
		return min + (double_h->Hash(win::system::PerformanceCounter::GetValue().QuadPart) / static_cast<double>(UINT64_MAX)) * range;
	}
}
