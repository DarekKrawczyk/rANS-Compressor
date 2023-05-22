#pragma once
#include "SymbolInformation.hpp"
#include <list>
#include <math.h>
#include <vector>

#define mod %

namespace rANS {
	class rANSCompressor
	{
	/* This is a rANS compressor class performing encoding and decoding
	   on provided data.
	*/
	public:
		rANSCompressor();
		rANSCompressor(const SymbolInformation& data);
		rANSCompressor(const SymbolInformation* data);
		~rANSCompressor();

		int encode(int* data, size_t dataSize);
		void decode(int value, std::list<int>& result);
		void decode(std::list<int>& result);

		void printData();
		//int encode(DataHolder data);
		//DataHolder decode(int value);

	private:
		SymbolInformation _data;
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