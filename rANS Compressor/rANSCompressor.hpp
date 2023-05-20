#pragma once
#include "DataHolder.hpp"
#include <list>
#include <math.h>
#include <vector>

#define mod %

namespace rANS {
	class rANSCompressor
	{
	public:
		rANSCompressor();
		~rANSCompressor();

		int encode(int* data, size_t dataSize);
		void decode(int value, std::list<int>& result);
		void decode(std::list<int>& result);

		int encode(DataHolder data);
		DataHolder decode(int value);

	private:
		std::vector<int> _alphabet;
		std::vector<int> _freq;
		std::vector<int> _cumul;
		int _M;
		int _encoderState;

	private:
		void encodeStep(int symbol);
		int decodeStep();
		int findNearestBin(int slot);
	};
}