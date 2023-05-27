#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

#define FRQ_SIZE 256
#define CDF_SIZE 257

namespace rANS {
	class SymbolInformation
	{
	/*  This class holds information about data to be compressed.
		Information such as: Data, Frequencies, Alphabet, M and 
	    Cumulative are calculated by this class.
	*/
	public:
		SymbolInformation();
		SymbolInformation(const std::string& dataBuffer);
		SymbolInformation(const SymbolInformation& other);
		SymbolInformation(const SymbolInformation* other);
		~SymbolInformation();

		std::string getBuffer() const;
		char getBuffer(int index) const;
		size_t getBufferSize() const;
		std::string getAlphabet() const;
		char getAlphabet(int index) const;

		uint32_t* getFrequencies() const;
		uint32_t getFrequencies(int index) const;
		uint32_t* getCumul() const;
		uint32_t getCumul(int index) const;

		uint32_t getD() const;

		size_t getAlphabetSize() const;
		int getM() const;
		int findNearestBin(int slot);

		bool loadData(const std::string& path);
		void calculateMetric();
		void printData();

		//JUST FOR TESTS
		char _symbols[1<<14];
	private:
		uint32_t n = 14;
		uint32_t scale = 1 << n;
		uint32_t mask = (1u << n) - 1;

		uint32_t _normalizationFactor = 23;
		uint32_t RENORM_LOW = pow(2, _normalizationFactor);
		uint32_t RENORM_HIGH = pow(2, 2*_normalizationFactor) - 1;

		std::string _dataBuffer;
		size_t _bufferSize;
		std::string _alphabet;
		uint32_t* _FRQ;
		uint32_t* _CDF;
		size_t _alphabetSize;
		uint32_t _M;
		uint32_t _D = 32 - n;
	};
}
