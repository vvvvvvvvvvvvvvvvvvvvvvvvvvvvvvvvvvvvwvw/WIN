#include "pch.h"

export module Hashers;

import Object;
import IHash;

export namespace win::hashes
{
	class MurmurHash : public interfaces::IHash<uint64_t>, Object<MurmurHash>
	{
	public:
		uint64_t Hash(uint64_t value) 
		{
			value ^= value >> 33;
			value *= 0xff51afd7ed558ccdULL;
			value ^= value >> 33;
			value *= 0xc4ceb9fe1a85ec53ULL;
			value ^= value >> 33;
			return value;
		}
	};

	class DoubleHash : public interfaces::IHash<double>, Object<DoubleHash>
	{
	public:
		double Hash(double value) override
		{
			uint64_t bits;
			memcpy(&bits, &value, sizeof(value));
			uint64_t x = bits;
			x ^= x >> 33;
			x *= 0xff51afd7ed558ccdULL;
			x ^= x >> 33;
			x *= 0xc4ceb9fe1a85ec53ULL;
			x ^= x >> 33;
			return x;
		}
	};
}