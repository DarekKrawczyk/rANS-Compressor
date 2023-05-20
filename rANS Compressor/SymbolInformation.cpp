#include "SymbolInformation.hpp"

rANS::SymbolInformation::SymbolInformation() {

}

rANS::SymbolInformation::~SymbolInformation() {

}

bool rANS::SymbolInformation::loadData(const std::string& path) {
	bool result = false;
	std::ifstream file(path);
	if (file.is_open()) {
		std::cout << path << " - file opened successfully!" << std::endl;

		size_t bufferSize = 0;
		char character;
		while (file.get(character)) {
			bufferSize++;
		}
		_dataBuffer = new char[bufferSize];

		int iterator = 0;
		while (file.get(character)) {
			_dataBuffer[0] = character;
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