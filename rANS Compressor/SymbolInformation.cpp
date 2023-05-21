#include "SymbolInformation.hpp"

rANS::SymbolInformation::SymbolInformation() {
	_M = 0;
	_alphabetSize = 256;
	_alphabet = new char[_alphabetSize] {0};
	_freq = new int[_alphabetSize] {0};
	_cumul = new int[_alphabetSize] {0};
}

rANS::SymbolInformation::SymbolInformation(SymbolInformation& other) {
	this->_M = other._M;
	this->_alphabetSize = other._alphabetSize;
	this->_bufferSize = other._bufferSize;

	this->_dataBuffer = other._dataBuffer;
	//size_t dataBufferLength = strlen(other._dataBuffer) + 1;
	//this->_dataBuffer = new char[dataBufferLength];
	//strcpy_s(this->_dataBuffer, dataBufferLength, other._dataBuffer);

	this->_alphabet = other._alphabet;
	//size_t alphabetLength = strlen(other._alphabet) + 1;
	//this->_alphabet = new char[alphabetLength];
	//strcpy_s(this->_alphabet, alphabetLength, other._alphabet);

	//size_t frequenciesLength = (sizeof(other._freq) / sizeof(other._freq[0]));
	this->_freq = new int[_alphabetSize];
	for (int i = 0; i < _alphabetSize; i++) {
		this->_freq[i] = other._freq[i];
	}

	//size_t cumulLength = (sizeof(other._cumul) / sizeof(other._cumul[0]));
	//this->_cumul = new int[cumulLength];
	//memcpy(this->_cumul, other._cumul, cumulLength);
	this->_cumul = new int[_alphabetSize];
	for (int i = 0; i < _alphabetSize; i++) {
		this->_cumul[i] = other._freq[i];
	}
}

rANS::SymbolInformation::~SymbolInformation() {
	delete[] _freq;
	delete[] _cumul;
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
		std::cout << "---------- Data ---------\n";
		//M
		std::string M = "M = {" + std::to_string(_M) + "}\n";
		std::cout << M;

		//Data size
		std::string bufforSize = "Buffor size = {" + std::to_string(_bufferSize) + "}\n";
		std::cout << bufforSize;

		//Data buffer
		std::string result = "Buffor = {";
		for (int i = 0; i < _bufferSize; i++) {
			char sign = _dataBuffer[i];
			result += _dataBuffer[i];
			result += (i + 1 == _bufferSize ? "" : ",");
		}
		result += "}\n";
		std::cout << result;

		//Alphabet
		std::string alphabet = "Alphabet = {";
		for (int i = 0; i < _alphabetSize; i++) {
			char sign = _alphabet[i];
			alphabet += _alphabet[i];
			alphabet += (i + 1 == _alphabetSize ? "" : ",");
		}
		alphabet += "}\n";
		std::cout << alphabet;

		//Alphabet
		std::string freq = "Frequencies = {";
		for (int i = 0; i < _alphabetSize; i++) {
			char sign = _freq[i];
			freq += _freq[i];
			freq += (i + 1 == _alphabetSize ? "" : ",");
		}
		freq += "}\n";
		std::cout << freq;

		//Cumul
		std::string cumul = "Cumul = {";
		for (int i = 0; i < _alphabetSize; i++) {
			char sign = _freq[i];
			cumul += _freq[i];
			cumul += (i + 1 == _alphabetSize ? "" : ",");
		}
		cumul += "}\n";
		std::cout << cumul;

		std::cout << "--------- Data end -------\n";
	}
}

void rANS::SymbolInformation::calculateMetric() {
	//Fill alphabet with values;
	for (int i = 0; i < _alphabetSize; i++) {
		char casted = (char)i;
		//_alphabet[i] = casted;
		_alphabet += casted;
	}

	//Calculate frequencies
	const size_t occurrences = 256;
	int occurrenceTab[occurrences] = { 0 };
	for (int i = 0; i < _bufferSize; i++) {
		occurrenceTab[_dataBuffer[i]]++;
	}

	//Check if buffor size is dividable.
	int optimalOccurrenceTab[occurrences] = { 0 };
	int dividor = 1;
	bool possible = true;
	for (int i = 2; i <= 9; i++) {
		if (_bufferSize % i == 0) {
			//Go through occurences and find out if it is possible to divide by that dividor.
			for (int j = 0; j < occurrences; j++) {
				if (occurrenceTab[j] % i == 0) {
					optimalOccurrenceTab[j] = occurrenceTab[j] / i;
					possible = true;
				}
				else {
					possible = false;
					break;
				}
			}
			if (possible == true) {
				dividor = i;
			}
		}
	}
	if (dividor > 1) {
		std::cout << "Optimalized by: " << dividor << std::endl;
		_M = _bufferSize / dividor;
		for (int i = 0; i < _alphabetSize; i++) {
			_freq[i] = optimalOccurrenceTab[i];
		}
	}
	else {
		std::cout << "Not optimalized!"<< std::endl;
		_M = _bufferSize;
		for (int i = 0; i < _alphabetSize; i++) {
			_freq[i] = optimalOccurrenceTab[i];
		}
	}

	//Calculate Cumulative
	for (int i = 0; i < _alphabetSize; i++) {
		if (i == 0) {
			_cumul[i] = 0;
			continue;
		}
		_cumul[i] = _cumul[i - 1] + _freq[i - 1];
	}
	std::cout << "Finished calculating!" << std::endl;
}