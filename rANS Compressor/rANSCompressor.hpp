#pragma once
#include "SymbolInformation.hpp"
#include "CompressionDetails.hpp"
#include <filesystem>
#include <sstream>
#include <list>
#include <math.h>
#include <chrono>
#include <fstream>

#define OUTPUT_FOLDER "data//"
#define STATE_LOGGER 0

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

		uint32_t encodeFile(std::string encodePath);
		uint32_t decodeFile(std::string decodePath, std::string symbolInfoPath);

		std::shared_ptr<std::list<uint8_t>> encode(const std::shared_ptr<std::list<uint8_t>>& dataBuffer, const SymbolInformation& info);
		std::shared_ptr<std::list<uint8_t>> decode(const SymbolInformation& info, const std::shared_ptr<std::list<uint8_t>>& encodedData);

		void printData();
		CompressionDetails getEncodingDetails() const;
		CompressionDetails getDecodingDetails() const;

	private:
		SymbolInformation _symbolInformation;
		uint32_t _encoderState;
		uint32_t _decoderState;
		std::shared_ptr<std::list<uint8_t>> _encodedBuffer;
		std::shared_ptr<std::list<uint8_t>> _decodedBuffer;
		CompressionDetails _encodingDetails;
		CompressionDetails _decodingDetails;
		std::ofstream _outputFile;
		std::ofstream _outputEncodingState;
		std::ofstream _outputDecodingState;
		std::string folder;

	private:
		void encodeStep(uint8_t symbol);
		void decodeStep();
		uint8_t read8bits();
	};
}