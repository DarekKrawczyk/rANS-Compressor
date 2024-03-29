#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <math.h>
#include <filesystem>
#include <list>

#define mod %
#define ALPHABET_SIZE 256			//Size of the alphabet. ASCII alphabet -> 256 symbols.
#define N 14						//Precision of represented probabilities.
#define NORMALIZATION_FACTOR 23		//Determine factor for lower and upper boundary of normalization.

namespace rANS {
	class SymbolInformation
	{
	/*  This class holds information about data to be compressed.
		Information such as: Data, Frequencies, Alphabet, M and 
	    Cumulative are calculated by this class.
	*/
	public:
		SymbolInformation();
		SymbolInformation(const std::shared_ptr<std::list<uint8_t>>& dataBuffer);
		SymbolInformation(const SymbolInformation& other);
		SymbolInformation(const SymbolInformation* other);
		~SymbolInformation();

		static std::shared_ptr<std::list<uint8_t>> LoadBuffer(std::string path);
		std::shared_ptr<std::list<uint8_t>> loadDataFromFile(std::string path);
		static std::shared_ptr<std::list<uint8_t>> ToBuffer(const std::string& data);
		void calculateMetric(const std::shared_ptr<std::list<uint8_t>>& dataBuffer);
		void toFile();
		bool loadSymbolInfoFromFile(std::string path);
		void printData();
		void calculateSymbolsInformation();
		void clearData();
		bool isEqual(const SymbolInformation& other);

		size_t getBufferSize() const;
		uint32_t getD() const;
		uint32_t getN() const;
		uint32_t getScale() const;
		uint32_t getMask() const;
		uint32_t getLowRenormBoundary() const;
		uint32_t getMaxEncoderState(uint8_t symbol) const;
		uint8_t getSymbol(uint32_t index) const;
		uint32_t getBias(uint8_t symbol) const;
		uint32_t getReciprocalFreq(uint8_t symbol) const;
		uint32_t getFreqComplement(uint8_t symbol) const;
		uint32_t getReciprocalShift(uint8_t symbol) const;
		uint32_t getFrequency(uint8_t symbol) const;
		uint32_t getCumulative(uint8_t symbol) const;
		uint8_t getAlphabet(uint32_t index) const;
		size_t getAlphabetSize() const;

	private:
		size_t bufferSize = 0;
		std::string _alphabet;

		uint32_t _n = N;
		uint32_t _scale = 1 << N;
		uint32_t _mask = (1u << N) - 1;
		uint32_t _d = (2 * NORMALIZATION_FACTOR) - N;

		uint32_t _normalizationFactor = NORMALIZATION_FACTOR;
		uint32_t _renormLow = (1u << NORMALIZATION_FACTOR);
		uint64_t _renormHigh = (1u << 2 * NORMALIZATION_FACTOR) - 1;

		uint8_t _symbols[1 << N]{ 0 };
		uint32_t _maxEncoderState[ALPHABET_SIZE]{ 0 };
		uint32_t _bias[ALPHABET_SIZE]{ 0 };
		uint32_t _reciprocalFreq[ALPHABET_SIZE]{ 0 };
		uint32_t _frequencyComplement[ALPHABET_SIZE]{ 0 };
		uint32_t _reciprocalShift[ALPHABET_SIZE]{ 0 };
		uint32_t _frequencies[ALPHABET_SIZE]{ 0 };
		uint32_t _cumulatives[ALPHABET_SIZE + 1]{ 0 };
	};
}
