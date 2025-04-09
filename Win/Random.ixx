#include "pch.h"

export module Random;

import Object;
import IHash;
import Hashers;
import Time;

win::hashes::MurmurHash mh;
win::hashes::DoubleHash dh;

export namespace win
{
	class Random : public Object<Random>
	{
	private:
		interfaces::IHash<uint64_t>* int_h = &mh;
		interfaces::IHash<double>* double_h = &dh;
	public:

		template<typename HT>
		void UINT_HASH(interfaces::IHash<HT>* custom_hash);

		template<typename HT>
		void DOUBLE_HASH(interfaces::IHash<HT>* custom_hash);

		int Next(int min, int max);
		double NextDouble(double min, double max);
	};

	template<typename HT>
	void Random::UINT_HASH(interfaces::IHash<HT>* custom_hash)
	{
		int_h = custom_hash;
	}
	template<typename HT>
	void Random::DOUBLE_HASH(interfaces::IHash<HT>* custom_hash)
	{
		double_h = custom_hash;
	}
}