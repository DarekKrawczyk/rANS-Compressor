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

		std::string encode(const SymbolInformation& data);
		std::string decode(const SymbolInformation& info, const std::string& encodedData);

		void decode(int value, std::list<int>& result);
		void decode(std::list<int>& result);

		void printData();

	private:
		uint32_t _msk = pow(2, 16) - 1;
		SymbolInformation _data;
		uint32_t _encoderState;
		uint32_t _decoderState;
		std::string _encodedBuffer;
		std::string _decodedBuffer;

	private:
		void encodeStep(int symbol);
		void decodeStepik();
		int decodeStep();
		int findNearestBin(int slot);
		void write16bits();
		uint32_t read16bits();
	};
}