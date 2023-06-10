#pragma once
#include "SymbolInformation.hpp"
#include "CompressionDetails.hpp"
#include <list>
#include <math.h>
#include <vector>
#include <chrono>
#include <fstream>

// Implmenentation 1 - own, 0 - rygorous
#define Implementation 0

namespace rANS {
	class rANSCompressor
	{
	/* This is a rANS compressor class performing encoding and decoding
	   on provided data.
	*/
	public:
		rANSCompressor();
		rANSCompressor(const SymbolInformation& info);
		rANSCompressor(const SymbolInformation* info);
		~rANSCompressor();

		uint32_t encodeFile(const std::string path);
		std::string encode(const SymbolInformation& info);
		std::string decode(const SymbolInformation& info, const std::string& encodedData);

		void printData();
		CompressionDetails getEncodingDetails() const;
		CompressionDetails getDecodingDetails() const;

	private:
		SymbolInformation _symbolInformation;
		uint32_t _encoderState;
		uint32_t _decoderState;
		std::string _encodedBuffer;
		std::string _decodedBuffer;
		CompressionDetails _encodingDetails;
		CompressionDetails _decodingDetails;
		std::ofstream _outputFile;

	private:
		void encodeStep(uint32_t symbol, bool toBuffer = true);
		void decodeStep();
		uint8_t read8bits();
		void write8bits(uint8_t buffer);
	};
}