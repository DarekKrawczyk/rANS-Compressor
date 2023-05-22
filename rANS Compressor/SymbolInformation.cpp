#include "SymbolInformation.hpp"

rANS::SymbolInformation::SymbolInformation() {
	_M = 0;
	_alphabetSize = 256;
	_alphabet = new char[_alphabetSize] {0};
	_freq = new int[_alphabetSize] {0};
	_cumul = new int[_alphabetSize] {0};
}

rANS::SymbolInformation::SymbolInformation(const SymbolInformation* other) {
	this->_M = other->_M;
	this->_alphabetSize = other->_alphabetSize;
	this->_bufferSize = other->_bufferSize;

	//Creating deep copies.
	this->_dataBuffer = other->_dataBuffer;
	this->_alphabet = other->_alphabet;

	_freq = new int[_alphabetSize];
	for (int i = 0; i < _alphabetSize; i++) {
		this->_freq[i] = other->_freq[i];
	}

	_cumul = new int[_alphabetSize];
	for (int i = 0; i < _alphabetSize; i++) {
		this->_cumul[i] = other->_cumul[i];

	}
}

rANS::SymbolInformation::SymbolInformation(const SymbolInformation& other) {
	this->_M = other._M;
	this->_alphabetSize = other._alphabetSize;
	this->_bufferSize = other._bufferSize;

	//Creating deep copies.
	this->_dataBuffer = other._dataBuffer;
	this->_alphabet = other._alphabet;

	_freq = new int[_alphabetSize];
	for (int i = 0; i < _alphabetSize; i++) {
		this->_freq[i] = other._freq[i];
	}

	_cumul = new int[_alphabetSize];
	for (int i = 0; i < _alphabetSize; i++) {
		this->_cumul[i] = other._cumul[i];

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
			int sign = _freq[i];
			freq += std::to_string(_freq[i]);
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
			int sign = _cumul[i];
			cumul += std::to_string(_cumul[i]);
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