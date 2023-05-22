#pragma once
#include <string>
#include <iostream>
#include <fstream>

namespace rANS {
	class SymbolInformation
	{
	/*  This class holds information about data to be compressed.
		Information such as: Data, Frequencies, Alphabet, M and 
	    Cumulative are calculated by this class.
	*/
	public:
		SymbolInformation();
		SymbolInformation(const SymbolInformation& other);
		SymbolInformation(const SymbolInformation* other);
		~SymbolInformation();

		bool loadData(const std::string& path);
		void calculateMetric();
		void printData();
	private:
		std::string _dataBuffer;
		size_t _bufferSize;
		std::string _alphabet;
		int* _freq;
		int* _cumul;
		size_t _alphabetSize;
		int _M;
	};
}
