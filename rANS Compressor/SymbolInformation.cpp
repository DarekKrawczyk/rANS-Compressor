#include "SymbolInformation.hpp"

rANS::SymbolInformation::SymbolInformation() {
	_dataBuffer = "";
	_bufferSize = _dataBuffer.length();
	_M = 0;
	_alphabetSize = FRQ_SIZE;
	_alphabet = "";
	_FRQ = new uint32_t[FRQ_SIZE] {0};

	//Adding one because last element is the denominator
	_CDF = new uint32_t[FRQ_SIZE + 1] {0};
	//_symbols = new char[scale]{ 0 };
}

rANS::SymbolInformation::SymbolInformation(const std::string& dataBuffer) {
	_dataBuffer = dataBuffer;
	_bufferSize = dataBuffer.length();
	_M = 0;
	_alphabetSize = FRQ_SIZE;
	_alphabet = "";
	_FRQ = new uint32_t[FRQ_SIZE] {0};
	_CDF = new uint32_t[FRQ_SIZE + 1] {0};
	//_symbols = new char[scale]{ 0 };
	this->calculateMetric();
}

rANS::SymbolInformation::SymbolInformation(const SymbolInformation* other) {
	this->_M = other->_M;
	this->_alphabetSize = other->_alphabetSize;
	this->_bufferSize = other->_bufferSize;

	//Creating deep copies.
	this->_dataBuffer = other->_dataBuffer;
	this->_alphabet = other->_alphabet;

	_FRQ = new uint32_t[_alphabetSize];
	for (int i = 0; i < _alphabetSize; i++) {
		this->_FRQ[i] = other->_FRQ[i];
	}

	_CDF = new uint32_t[_alphabetSize];
	for (int i = 0; i < _alphabetSize; i++) {
		this->_CDF[i] = other->_CDF[i];

	}
}

rANS::SymbolInformation::SymbolInformation(const SymbolInformation& other) {
	this->_M = other._M;
	this->_alphabetSize = other._alphabetSize;
	this->_bufferSize = other._bufferSize;

	//Creating deep copies.
	this->_dataBuffer = other._dataBuffer;
	this->_alphabet = other._alphabet;

	_FRQ = new uint32_t[_alphabetSize];
	for (int i = 0; i < _alphabetSize; i++) {
		this->_FRQ[i] = other._FRQ[i];
	}

	_CDF = new uint32_t[_alphabetSize];
	for (int i = 0; i < _alphabetSize; i++) {
		this->_CDF[i] = other._CDF[i];

	}
}

rANS::SymbolInformation::~SymbolInformation() {
	delete[] _FRQ;
	delete[] _CDF;
	delete[] _symbols;
}

std::string rANS::SymbolInformation::getBuffer() const
{
	return _dataBuffer;
}

char rANS::SymbolInformation::getBuffer(int index) const
{
	return _dataBuffer[index];
}

size_t rANS::SymbolInformation::getBufferSize() const
{
	return _bufferSize;
}

std::string rANS::SymbolInformation::getAlphabet() const
{
	return _alphabet;
}

char rANS::SymbolInformation::getAlphabet(int index) const
{
	return _alphabet[index];
}

uint32_t* rANS::SymbolInformation::getFrequencies() const
{
	return _FRQ;
}

uint32_t rANS::SymbolInformation::getFrequencies(int index) const
{
	return _FRQ[index];
}

uint32_t* rANS::SymbolInformation::getCumul() const
{
	return _CDF;
}

uint32_t rANS::SymbolInformation::getCumul(int index) const
{
	return _CDF[index];
}

uint32_t rANS::SymbolInformation::getD() const
{
	return _D;
}

size_t rANS::SymbolInformation::getAlphabetSize() const
{
	return _alphabetSize;
}

int rANS::SymbolInformation::getM() const
{
	return _M;
}

int rANS::SymbolInformation::findNearestBin(int slot) {
	for (int i = 0; i < _alphabetSize; i++) {
		if (_CDF[i] == slot) {
			return i;
		}
		else if (_CDF[i] > slot) {
			return i - 1;
		}
	}
	return 0;
}

bool rANS::SymbolInformation::loadData(const std::string& path) {
	bool result = false;
	std::ifstream file(path);
	if (file.is_open()) {
		std::cout << path << " - file opened successfully!" << std::endl;

		//Get number of symbols in file.
		file.seekg(0, std::ios_base::end);
		size_t bufferSize = file.tellg();
		file.seekg(0, std::ios_base::beg);

		_bufferSize = bufferSize;
		_dataBuffer = "";

		//Read symbols from file.
		char character;
		int iterator = 0;
		while (file.get(character)) {
			_dataBuffer += character;
			iterator++;
		}

		result = true;
		file.close();
	}
	else {
		std::cout << path << " - failed to open the file!!!" << std::endl;
	}
	return result;
}

void rANS::SymbolInformation::printData() {
	if (_dataBuffer.length() <= 0) {
		std::cout << "Data buffer is empty!\n";
	}
	else {
		std::cout << "\n------------------- Data begin --------------------\n";
		//M
		std::string M = "M = {" + std::to_string(_M) + "}\n";
		std::cout << M;

		//Data size
		std::string bufforSize = "Buffor size = {" + std::to_string(_bufferSize) + "}\n";
		std::cout << bufforSize;

		//Data buffer
		std::string result = "-------------------- Buffor --------------------\n";
		for (int i = 0; i < _bufferSize; i++) {
			char sign = _dataBuffer[i];
			result += _dataBuffer[i];
			result += (i + 1 == _bufferSize ? "" : ",");
			if (i > 0 and i % 30 == 0) {
				result += "\n";
			}
		}
		result += "\n--------------------------------------------------\n\n\n";
		std::cout << result;

		//Alphabet
		std::string alphabet = "-------------------- Alphabet --------------------\n";
		for (int i = 0; i < _alphabetSize; i++) {
			char sign = _alphabet[i];
			alphabet += _alphabet[i];
			alphabet += (i + 1 == _alphabetSize ? "" : ",");
			if (i > 0 and i % 30 == 0) {
				alphabet += "\n";
			}
		}
		alphabet += "\n--------------------------------------------------\n\n\n";
		std::cout << alphabet;

		//Frequencies
		std::string freq = "-------------------- Frequencies --------------------\n";
		for (int i = 0; i < _alphabetSize; i++) {
			int sign = _FRQ[i];
			freq += std::to_string(_FRQ[i]);
			freq += (i + 1 == _alphabetSize ? "" : ",");
			if (i > 0 and i % 30 == 0) {
				freq += "\n";
			}
		}
		freq += "\n--------------------------------------------------\n\n\n";
		std::cout << freq;

		//Cumul
		std::string cumul = "-------------------- Cumul --------------------\n";
		for (int i = 0; i < _alphabetSize; i++) {
			int sign = _CDF[i];
			cumul += std::to_string(_CDF[i]);
			cumul += (i + 1 == _alphabetSize ? "" : ",");
			if (i > 0 and i % 30 == 0) {
				cumul += "\n";
			}
		}
		cumul += "\n--------------------------------------------------\n";
		std::cout << cumul;

		std::cout << "\n------------------- Data end----------------------\n\n\n";
	}
}

void rANS::SymbolInformation::calculateMetric() {

	_bufferSize = _dataBuffer.length();
	//Fill alphabet with values;
	for (int i = 0; i < _alphabetSize; i++) {
		char casted = (char)i;
		//_alphabet[i] = casted;
		_alphabet += casted;
	}

	//Calculate frequencies
	//const size_t occurrences = 256;
	//int occurrenceTab[occurrences] = { 0 };
	for (int i = 0; i < _bufferSize; i++) {
		int value = _dataBuffer[i];
		int value2 = _FRQ[_dataBuffer[i]];
		_FRQ[_dataBuffer[i]]++;
	}

	_CDF[0] = 0;
	for (int i = 0; i < 256; i++) {
		_CDF[i + 1] = _CDF[i] + _FRQ[i];
	}

	_M = _CDF[256];

	int test[256];
	for (int i = 0; i < 256; i++) {
		test[i] = _FRQ[i];
	}

	// resample distribution based on cumulative freqs
	for (int i = 1; i <= 256; i++)
		_CDF[i] = ((uint64_t)scale * _CDF[i]) / _M;

	// if we nuked any non-0 frequency symbol to 0, we need to steal
	// the range to make the frequency nonzero from elsewhere.
	//
	// this is not at all optimal, i'm just doing the first thing that comes to mind.
	for (int i = 0; i < 256; i++) {
		if (_FRQ[i] && _CDF[i + 1] == _CDF[i]) {
			// symbol i was set to zero freq

			// find best symbol to steal frequency from (try to steal from low-freq ones)
			uint32_t best_freq = ~0u;
			int best_steal = -1;
			for (int j = 0; j < 256; j++) {
				uint32_t freq = _CDF[j + 1] - _CDF[j];
				if (freq > 1 && freq < best_freq) {
					best_freq = freq;
					best_steal = j;
				}
			}

			// and steal from it!
			if (best_steal < i) {
				for (int j = best_steal + 1; j <= i; j++)
					_CDF[j]--;
			}
			else {
				for (int j = i + 1; j <= best_steal; j++)
					_CDF[j]++;
			}
		}
	}

	_M = _CDF[256];

	// calculate updated freqs and make sure we didn't screw anything up
	for (int i = 0; i < 256; i++) {
		// calc updated freq
		_FRQ[i] = _CDF[i + 1] - _CDF[i];
	}

	////Check if buffor size is dividable.
	//int optimalOccurrenceTab[occurrences] = { 0 };
	//int dividor = 1;
	//bool possible = true;
	//for (int i = 2; i <= 9; i++) {
	//	if (_bufferSize % i == 0) {
	//		//Go through occurences and find out if it is possible to divide by that dividor.
	//		for (int j = 0; j < occurrences; j++) {
	//			if (occurrenceTab[j] % i == 0) {
	//				optimalOccurrenceTab[j] = occurrenceTab[j] / i;
	//				possible = true;
	//			}
	//			else {
	//				possible = false;
	//				break;
	//			}
	//		}
	//		if (possible == true) {
	//			dividor = i;
	//		}
	//	}
	//}
	//if (dividor > 1) {
	//	std::cout << "Optimalized by: " << dividor << std::endl;
	//	_M = _bufferSize / dividor;
	//	for (int i = 0; i < _alphabetSize; i++) {
	//		_FRQ[i] = optimalOccurrenceTab[i];
	//	}
	//}
	//else {
	//	std::cout << "Not optimalized!"<< std::endl;
	//	_M = _bufferSize;
	//	for (int i = 0; i < _alphabetSize; i++) {
	//		_FRQ[i] = occurrenceTab[i];
	//	}
	//}

	////Calculate Cumulative
	//for (int i = 0; i < _alphabetSize; i++) {
	//	if (i == 0) {
	//		_CDF[i] = 0;
	//		continue;
	//	}
	//	_CDF[i] = _CDF[i - 1] + _FRQ[i - 1];
	//}


	for (int s = 0; s < 256; s++) {
		for (uint32_t i = _CDF[s]; i < _CDF[s + 1]; i++) {
			_symbols[i] = s;
		}
	}
		
	//For debug
	int tabF[256];
	int tabC[256];
	int tabS[1<<14];
	for (int i = 0; i < _alphabetSize; i++) {
		tabF[i] = _FRQ[i];
		tabC[i] = _CDF[i];
	}
	for (int i = 0; i < scale; i++) {
		tabS[i] = _symbols[i];
	}

	std::cout << "Finished calculating!" << std::endl;
}