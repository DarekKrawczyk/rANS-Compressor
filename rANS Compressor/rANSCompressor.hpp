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

		int encode(const SymbolInformation& data);
		void decode(int value, std::list<int>& result);
		void decode(std::list<int>& result);

		void printData();
		//int encode(DataHolder data);
		//DataHolder decode(int value);

	private:
		uint32_t _msk = pow(2, 16) - 1;
		SymbolInformation _data;
		uint32_t _outBufferIterator;
		uint8_t* _outBuffer;
		uint32_t _encoderState;

	private:
		void encodeStep(int symbol);
		int decodeStep();
		int findNearestBin(int slot);
		void write16bits();
	};
}