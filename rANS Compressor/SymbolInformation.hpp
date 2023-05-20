#pragma once
#include <string>
#include <iostream>
#include <fstream>

namespace rANS {
	class SymbolInformation
	{
		/* This class holds information about data to be compressed.
		   Information such as: Data, Frequencies, Alphabet, M and 
		   Cumulative are calculated by this class.
		*/
	public:
		SymbolInformation();
		~SymbolInformation();

		bool loadData(const std::string& path);
	private:
		char* _dataBuffer;
	};
}
