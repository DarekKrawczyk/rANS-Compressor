#pragma once
#include "SymbolInformation.hpp"
#include "CompressionDetails.hpp"
#include <filesystem>
#include <sstream>
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

		uint32_t encodeFile(std::string pathOfFileToEncode);
		uint32_t decodeFile(std::string pathOfFileToDecode, std::string pathOfFileWithSymbolInfo);

		std::string encode(const std::string& datBuffer, const SymbolInformation& info);
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
		void encodeStep(uint8_t symbol, bool toBuffer = true);
		void decodeStep();
		uint8_t read8bits();
		void write8bits(uint8_t buffer);
	};
}