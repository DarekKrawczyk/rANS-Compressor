#include "SymbolInformation.hpp"

#pragma region Constructors and Destructor
/// <summary>
/// Default constructor.
/// </summary>
rANS::SymbolInformation::SymbolInformation() {
	//_dataBuffer = "";
	bufferSize = 0;
	_alphabet = "";
}

/// <summary>
/// Default constructor, load data buffer and calculates metric of the symbols.
/// </summary>
/// <param name="dataBuffer">Data</param>
rANS::SymbolInformation::SymbolInformation(const std::string& dataBuffer) {
	//_dataBuffer = dataBuffer;
	_alphabet = "";
	bufferSize = dataBuffer.length();
	this->calculateMetric(dataBuffer);
}

/// <summary>
/// Copy constructor.
/// </summary>
/// <param name="other">Object from which to copy data.</param>
rANS::SymbolInformation::SymbolInformation(const SymbolInformation* other) {
	//Coping buffer and alphabet.
	//this->_dataBuffer = other->_dataBuffer;
	this->bufferSize = other->bufferSize;
	this->_alphabet = other->_alphabet;

	//Coping frequencies.
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		this->_frequencies[i] = other->_frequencies[i];
		this->_cumulatives[i] = other->_cumulatives[i];
		this->_maxEncoderState[i] = other->_maxEncoderState[i];
		this->_bias[i] = other->_bias[i];
		this->_reciprocalFreq[i] = other->_reciprocalFreq[i];
		this->_frequencyComplement[i] = other->_frequencyComplement[i];
		this->_reciprocalShift[i] = other->_reciprocalShift[i];
		this->_frequencies[i] = other->_frequencies[i];
	}

	//Coping cumulative.
	for (int i = 0; i < (ALPHABET_SIZE + 1); i++) {
		this->_cumulatives[i] = other->_cumulatives[i];
	}

	//Coping symbols.
	for (int i = 0; i < (1<<N); i++) {
		this->_symbols[i] = other->_symbols[i];
	}
}

/// <summary>
/// Copy constructor.
/// </summary>
/// <param name="other">Object from which to copy data.</param>
rANS::SymbolInformation::SymbolInformation(const SymbolInformation& other) {
	//Coping buffer and alphabet.
	//this->_dataBuffer = other._dataBuffer;
	this->bufferSize = other.bufferSize;
	this->_alphabet = other._alphabet;

	//Coping frequencies.
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		this->_frequencies[i] = other._frequencies[i];
		this->_cumulatives[i] = other._cumulatives[i];
		this->_maxEncoderState[i] = other._maxEncoderState[i];
		this->_bias[i] = other._bias[i];
		this->_reciprocalFreq[i] = other._reciprocalFreq[i];
		this->_frequencyComplement[i] = other._frequencyComplement[i];
		this->_reciprocalShift[i] = other._reciprocalShift[i];
		this->_frequencies[i] = other._frequencies[i];
	}

	//Coping cumulative.
	for (int i = 0; i < (ALPHABET_SIZE + 1); i++) {
		this->_cumulatives[i] = other._cumulatives[i];
	}

	//Coping symbols.
	for (int i = 0; i < (1 << N); i++) {
		this->_symbols[i] = other._symbols[i];
	}
}

/// <summary>
/// Destructor.
/// </summary>
rANS::SymbolInformation::~SymbolInformation() {

}
#pragma endregion
#pragma region Accessors
//std::string rANS::SymbolInformation::getBuffer() const
//{
//	return _dataBuffer;
//}
//
//char rANS::SymbolInformation::getBuffer(int index) const
//{
//	return _dataBuffer[index];
//}
//
size_t rANS::SymbolInformation::getBufferSize() const
{
	return bufferSize;
}

uint8_t rANS::SymbolInformation::getAlphabet(uint32_t index) const
{
	return _alphabet[index];
}

uint32_t rANS::SymbolInformation::getFrequency(uint8_t symbol) const
{
	return _frequencies[symbol];
}

uint32_t rANS::SymbolInformation::getCumulative(uint8_t symbol) const
{
	return _cumulatives[symbol];
}

uint32_t rANS::SymbolInformation::getD() const
{
	return _d;
}

size_t rANS::SymbolInformation::getAlphabetSize() const
{
	return ALPHABET_SIZE;
}

uint32_t rANS::SymbolInformation::getN() const
{
	return _n;
}

uint32_t rANS::SymbolInformation::getScale() const
{
	return _scale;
}

uint32_t rANS::SymbolInformation::getMask() const
{
	return _mask;
}

uint32_t rANS::SymbolInformation::getLowRenormBoundary() const
{
	return _renormLow;
}

uint32_t rANS::SymbolInformation::getMaxEncoderState(uint8_t symbol) const
{
	return _maxEncoderState[symbol];
}

uint8_t rANS::SymbolInformation::getSymbol(uint32_t index) const
{
	return _symbols[index];
}

uint32_t rANS::SymbolInformation::getBias(uint8_t symbol) const
{
	return _bias[symbol];
}

uint32_t rANS::SymbolInformation::getReciprocalFreq(uint8_t symbol) const
{
	return _reciprocalFreq[symbol];
}

uint32_t rANS::SymbolInformation::getFreqComplement(uint8_t symbol) const
{
	return _frequencyComplement[symbol];
}

uint32_t rANS::SymbolInformation::getReciprocalShift(uint8_t symbol) const
{
	return _reciprocalShift[symbol];
}
#pragma endregion
#pragma region Public methods
/// <summary>
/// Clear all data of the object.
/// </summary>
void rANS::SymbolInformation::clearData() {
	//Clear data buffer.
	//_dataBuffer.clear();
	_alphabet.clear();

	//Clear all of the symbol information.
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		_maxEncoderState[i] = 0;
		_bias[i] = 0;
		_reciprocalFreq[i] = 0;
		_frequencyComplement[i] = 0;
		_reciprocalShift[i] = 0;
		_frequencies[i] = 0;
	}	
	
	for (int i = 0; i < ALPHABET_SIZE + 1; i++) {
		_cumulatives[i] = 0;
	}

	for (int i = 0; i < (1 << N); i++) {
		_symbols[i] = 0;
	}
}

bool rANS::SymbolInformation::isEqual(const SymbolInformation& other)
{
	if (this->bufferSize != other.bufferSize) {
		return false;
	}
	if (this->_alphabet != other._alphabet) {
		return false;
	}
	if (this->_n != other._n) {
		return false;
	}
	if (this->_scale != other._scale) {
		return false;
	}
	if (this->_mask != other._mask) {
		return false;
	}
	if (this->_d != other._d) {
		return false;
	}
	if (this->_normalizationFactor != other._normalizationFactor) {
		return false;
	}
	if (this->_renormLow != other._renormLow) {
		return false;
	}
	if (this->_renormHigh != other._renormHigh) {
		return false;
	}
	for (int i = 0; i < (1 << N); i++) {
		uint8_t first = this->_symbols[i];
		uint8_t sec = other._symbols[i];
		if (this->_symbols[i] != other._symbols[i]) {
			return false;
		}
	}
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (this->_maxEncoderState[i] != other._maxEncoderState[i] or
			this->_bias[i] != other._bias[i] or
			this->_reciprocalFreq[i] != other._reciprocalFreq[i] or
			this->_frequencyComplement[i] != other._frequencyComplement[i] or
			this->_reciprocalShift[i] != other._reciprocalShift[i] or
			this->_frequencies[i] != other._frequencies[i] or
			this->_cumulatives[i] != other._cumulatives[i]) {
			return false;
		}
	}
	for (int i = 0; i < (ALPHABET_SIZE + 1); i++) {
		if (this->_cumulatives[i] != other._cumulatives[i]) {
			return false;
		}
	}
	return true;
}

/// <summary>
/// Load data from file and calculate it's metric.
/// </summary>
/// <param name="path">Path of the file.</param>
/// <returns>True if operation was successful, otherwise false.</returns>
std::shared_ptr<std::string> rANS::SymbolInformation::loadDataFromFile(std::string path) {
	std::shared_ptr<std::string> buffer = std::make_shared<std::string>();
	std::ifstream file(path, std::ios_base::binary);
	if (file.is_open()) {
		//Read symbols from file.
		*buffer = std::string((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		bufferSize = buffer->length();
		file.close();
		this->calculateMetric(*buffer);
	}
	else {
		std::cout << path << " - failed to open the file!!!" << std::endl;
	}
	return buffer;
}

/*
std::shared_ptr<std::string> rANS::SymbolInformation::loadDataFromFile(std::string path) {
	bool result = false;
	std::shared_ptr<std::string> buffer = std::make_shared<std::string>();
	std::ifstream file(path, std::ios_base::binary);
	if (file.is_open()) {
		//std::cout << path << " - file opened successfully!" << std::endl;

		//Read symbols from file.
		*buffer = "";
		char character;
		while (file.get(character)) {
			*buffer += character;
		}

		bufferSize = buffer->length();
		result = true;
		file.close();
		this->calculateMetric(*buffer);
	}
	else {
		std::cout << path << " - failed to open the file!!!" << std::endl;
	}
	return buffer;
}


*/

/// <summary>
/// Print all information about the symbols.
/// </summary>
void rANS::SymbolInformation::printData() {
		std::cout << "\n------------------- Data begin --------------------\n";
		//M
		//std::string M = "M = {" + std::to_string(_M) + "}\n";
		//std::cout << M;

		//Data size
		//std::string bufforSize = "Buffor size = {" + std::to_string(_dataBuffer.length()) + "}\n";
		//std::cout << bufforSize;

		////Data buffer
		//std::string result = "-------------------- Buffor --------------------\n";
		//for (int i = 0; i < _dataBuffer.length(); i++) {
		//	char sign = _dataBuffer[i];
		//	result += _dataBuffer[i];
		//	result += (i + 1 == _dataBuffer.length() ? "" : ",");
		//	if (i > 0 and i % 30 == 0) {
		//		result += "\n";
		//	}
		//}
		//result += "\n--------------------------------------------------\n\n\n";
		//std::cout << result;

		//Alphabet
		std::string alphabet = "-------------------- Alphabet --------------------\n";
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			char sign = _alphabet[i];
			alphabet += _alphabet[i];
			alphabet += (i + 1 == ALPHABET_SIZE ? "" : ",");
			if (i > 0 and i % 30 == 0) {
				alphabet += "\n";
			}
		}
		alphabet += "\n--------------------------------------------------\n\n\n";
		std::cout << alphabet;

		//Frequencies
		std::string freq = "-------------------- Frequencies --------------------\n";
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			int sign = _frequencies[i];
			freq += std::to_string(_frequencies[i]);
			freq += (i + 1 == ALPHABET_SIZE ? "" : ",");
			if (i > 0 and i % 30 == 0) {
				freq += "\n";
			}
		}
		freq += "\n--------------------------------------------------\n\n\n";
		std::cout << freq;

		//Cumul
		std::string cumul = "-------------------- Cumul --------------------\n";
		for (int i = 0; i < ALPHABET_SIZE; i++) {
			int sign = _cumulatives[i];
			cumul += std::to_string(_cumulatives[i]);
			cumul += (i + 1 == ALPHABET_SIZE ? "" : ",");
			if (i > 0 and i % 30 == 0) {
				cumul += "\n";
			}
		}
		cumul += "\n--------------------------------------------------\n";
		std::cout << cumul;

		std::cout << "\n------------------- Data end----------------------\n\n\n";
}

/// <summary>
/// Calculate metric for symbols.
/// </summary>
void rANS::SymbolInformation::calculateMetric(const std::string& dataBuffer) {
	//Fill alphabet with default values.
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		_alphabet += (uint8_t)i;
	}

	//Calculate frequencies.
	for (int i = 0; i < dataBuffer.length(); i++) {
		_frequencies[(uint8_t)dataBuffer[i]]++;
	}

	//Calculate cumulative frequencies.
	_cumulatives[0] = 0;
	for (int i = 0; i < 256; i++) {
		_cumulatives[i + 1] = _cumulatives[i] + _frequencies[i];
	}

	//Last cumulative frequency, used as divider.
	uint32_t m = _cumulatives[256];

	//rygorous code to resample, optimize and calculate frequencies and cumulative frequencies.
	// resample distribution based on cumulative freqs
	for (int i = 1; i <= 256; i++)
		_cumulatives[i] = ((uint64_t)_scale * _cumulatives[i]) / m;

	// if we nuked any non-0 frequency symbol to 0, we need to steal
	// the range to make the frequency nonzero from elsewhere.
	//
	// this is not at all optimal, i'm just doing the first thing that comes to mind.
	for (int i = 0; i < 256; i++) {
		if (_frequencies[i] && _cumulatives[i + 1] == _cumulatives[i]) {
			// symbol i was set to zero freq

			// find best symbol to steal frequency from (try to steal from low-freq ones)
			uint32_t best_freq = ~0u;
			int best_steal = -1;
			for (int j = 0; j < 256; j++) {
				uint32_t freq = _cumulatives[j + 1] - _cumulatives[j];
				if (freq > 1 && freq < best_freq) {
					best_freq = freq;
					best_steal = j;
				}
			}

			// and steal from it!
			if (best_steal < i) {
				for (int j = best_steal + 1; j <= i; j++)
					_cumulatives[j]--;
			}
			else {
				for (int j = i + 1; j <= best_steal; j++)
					_cumulatives[j]++;
			}
		}
	}

	m = _cumulatives[256];

	// calculate updated freqs and make sure we didn't screw anything up
	for (int i = 0; i < 256; i++) {
		// calc updated freq
		_frequencies[i] = _cumulatives[i + 1] - _cumulatives[i];
	}

	//My attempt to optimize frequencies and cumulative frequencies.

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
	//		_frequencies[i] = optimalOccurrenceTab[i];
	//	}
	//}
	//else {
	//	std::cout << "Not optimalized!"<< std::endl;
	//	_M = _bufferSize;
	//	for (int i = 0; i < _alphabetSize; i++) {
	//		_frequencies[i] = occurrenceTab[i];
	//	}
	//}

	////Calculate Cumulative
	//for (int i = 0; i < _alphabetSize; i++) {
	//	if (i == 0) {
	//		_cumulatives[i] = 0;
	//		continue;
	//	}
	//	_cumulatives[i] = _cumulatives[i - 1] + _frequencies[i - 1];
	//}


	for (int s = 0; s < 256; s++) {
		for (uint32_t i = _cumulatives[s]; i < _cumulatives[s + 1]; i++) {
			_symbols[i] = s;
		}
	}
	
	//Calculate other information about symbols.
	this->calculateSymbolsInformation();

	//For debug
	uint32_t tabF[256];
	uint32_t tabC[256];
	uint32_t tabTheSame[256];
	uint32_t tabTheSame2[256];
	uint32_t tabS[1<<N];
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		tabF[i] = _frequencies[i];
		tabC[i] = _cumulatives[i];
		tabTheSame[i] = _frequencyComplement[i];
		tabTheSame2[i] = _reciprocalFreq[i];
	}
	for (int i = 0; i < _scale; i++) {
		tabS[i] = _symbols[i];
	}

	//std::cout << "Finished calculating!" << std::endl;
}
void rANS::SymbolInformation::toFile(std::string path)
{
	std::ofstream file("symbolInformations.txt");

	file << "N {" << _n << "}";
	file << "\nBuffer size {" << bufferSize << "}";
	//file << "\nScale {" << _scale << "}";
	//file << "\nMask {" << _mask << "}";
	//file << "\nD {" << _d << "}";
	file << "\nRenormalization factor {" << _normalizationFactor << "}";
	file << "\nRenorm high {" << _renormHigh << "}";
	//file << "\nRenorm LOW {" << _renormLow << "}";
	//file << "\nRenorm HIGH {" << _renormHigh << "}";

	//Save alphabet,
	file << "\nAlphabet {";
	size_t alphabetSize = _alphabet.length();
	for (uint32_t i = 0; i < alphabetSize; i++) {
		file << i << (i == (alphabetSize) - 1 ? "}" : ","); //TODO: change this to proper alphabet save.
	}

	//Save symbols.
	file << "\nSymbols {";
	for (int i = 0; i < (1 << N); i++) {
		file << (int)_symbols[i] << (i == (1 << N) - 1 ? "}" : ",");
	}

	//Save frequencies
	file << "\nFrequenies {";
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		file << _frequencies[i] << (i == ALPHABET_SIZE - 1 ? "}" : ",");
	}

	//Save cumulatives
	file << "\nCumulatives {";
	for (int i = 0; i < ALPHABET_SIZE + 1; i++) {
		file << _cumulatives[i] << (i == (ALPHABET_SIZE) ? "}" : ",");
	}

	//Save max encode state
	file << "\nMax encoder state {";
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		file << _maxEncoderState[i] << (i == ALPHABET_SIZE - 1 ? "}" : ",");
	}

	//Save bias
	file << "\nBias {";
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		file << _bias[i] << (i == ALPHABET_SIZE - 1 ? "}" : ",");
	}

	//Save Reciprocal freqencies
	file << "\nMax reciprocal frequencies {";
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		file << _reciprocalFreq[i] << (i == ALPHABET_SIZE - 1 ? "}" : ",");
	}

	//Save frequencies complement  
	file << "\nFrequencies complement {";
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		file << _frequencyComplement[i] << (i == ALPHABET_SIZE - 1 ? "}" : ",");
	}

	//Save reciprocal shift
	file << "\nReciprocal shift{";
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		file << _reciprocalShift[i] << (i == ALPHABET_SIZE - 1 ? "}" : ",");
	}

	file.close();
}
bool rANS::SymbolInformation::loadSymbolInfoFromFile(std::string path)
{
	bool result = false;
	std::ifstream file(path);
	if (file.is_open()) {
		//std::cout << path << " - file opened successfully!" << std::endl;

		//Read symbols from file.
		std::string dataBuffer = "";
		std::string temp = "";
		_alphabet.clear();
		uint8_t tempChar;
		int index = 0;
		char character;
		while (file.get(character)) {
			if (dataBuffer == "N {") {
				if (character != '}') {
					temp += character;
				}
				else {
					_n = (uint32_t)stoi(temp);
					temp = "";
					dataBuffer = "";
				}
			}
			else if (dataBuffer == "\nAlphabet {") {
				if (character != '}') {
					if (character != ',') {
						temp += character;
					}
					else {
						_alphabet += (uint8_t)stoi(temp);
						temp = "";
					}
				}
				else {
					_alphabet += (uint8_t)stoi(temp);
					temp = "";
					dataBuffer = "";
				}
			}
			else if (dataBuffer == "\nBuffer size {") {
				if (character != '}') {
					temp += character;
				}
				else {
					bufferSize = (uint32_t)stoi(temp);
					temp = "";
					dataBuffer = "";
				}
			}
			else if (dataBuffer == "\nRenormalization factor {") {
				if (character != '}') {
					temp += character;
				}
				else {
					_normalizationFactor = (uint32_t)stoi(temp);

					//Calculate other parameters based on N and renorm factor.
					_scale = 1 << _n;
					_mask = (1u << _n) - 1;
					_d = (2 * _normalizationFactor) - _n;
					_renormLow = (1u << _normalizationFactor);

					temp = "";
					dataBuffer = "";
				}
			}
			else if (dataBuffer == "\nRenorm high {") {
				if (character != '}') {
					temp += character;
				}
				else {
					_renormHigh = (uint64_t)stoul(temp);

					temp = "";
					dataBuffer = "";
				}
			}
			else if (dataBuffer == "\nSymbols {") {
				if (character != '}') {
					if (character != ',') {
						temp += character;
						//tempChar = character;
					}
					else {
						_symbols[index] = (uint64_t)stoi(temp);
						index++;
						temp = "";
					}
				}
				else {
					_symbols[index] = (uint64_t)stoi(temp);
					index = 0;
					temp = "";
					dataBuffer = "";
				}
			}
			else if (dataBuffer == "\nFrequenies {") {
				if (character != '}') {
					if (character != ',') {
						temp += character;
					}
					else {
						_frequencies[index] = (uint32_t)stoi(temp);
						index++;
						temp = "";
					}
				}
				else {
					_frequencies[index] = (uint32_t)stoi(temp);
					index = 0;
					temp = "";
					dataBuffer = "";
				}
			}
			else if (dataBuffer == "\nCumulatives {") {
				if (character != '}') {
					if (character != ',') {
						temp += character;
					}
					else {
						_cumulatives[index] = (uint32_t)stoi(temp);
						index++;
						temp = "";
					}
				}
				else {
					_cumulatives[index] = (uint32_t)stoi(temp);
					index = 0;
					temp = "";
					dataBuffer = "";
				}
			}			
			else if (dataBuffer == "\nMax encoder state {") {
				if (character != '}') {
					if (character != ',') {
						temp += character;
					}
					else {
						_maxEncoderState[index] = (uint32_t)stoi(temp);
						index++;
						temp = "";
					}
				}
				else {
					_maxEncoderState[index] = (uint32_t)stoi(temp);
					index = 0;
					temp = "";
					dataBuffer = "";
				}
			}			
			else if (dataBuffer == "\nBias {") {
				if (character != '}') {
					if (character != ',') {
						temp += character;
					}
					else {
						_bias[index] = (uint32_t)stoi(temp);
						index++;
						temp = "";
					}
				}
				else {
					_bias[index] = (uint32_t)stoi(temp);
					index = 0;
					temp = "";
					dataBuffer = "";
				}
			}			
			else if (dataBuffer == "\nMax reciprocal frequencies {") {
				if (character != '}') {
					if (character != ',') {
						temp += character;
					}
					else {
						_reciprocalFreq[index] = (uint32_t)stoul(temp);
						index++;
						temp = "";
					}
				}
				else {
					_reciprocalFreq[index] = (uint32_t)stoul(temp);
					index = 0;
					temp = "";
					dataBuffer = "";
				}
			}			
			else if (dataBuffer == "\nFrequencies complement {") {
				if (character != '}') {
					if (character != ',') {
						temp += character;
					}
					else {
						_frequencyComplement[index] = (uint32_t)stoi(temp);
						index++;
						temp = "";
					}
				}
				else {
					_frequencyComplement[index] = (uint32_t)stoi(temp);
					index = 0;
					temp = "";
					dataBuffer = "";
				}
			}			
			else if (dataBuffer == "\nReciprocal shift{") {
				if (character != '}') {
					if (character != ',') {
						temp += character;
					}
					else {
						_reciprocalShift[index] = (uint32_t)stoi(temp);
						index++;
						temp = "";
					}
				}
				else {
					_reciprocalShift[index] = (uint32_t)stoi(temp);
					index = 0;
					temp = "";
					dataBuffer = "";
				}
			}
			else dataBuffer += character;
		}

		result = true;
		file.close();
	}
	else {
		std::cout << path << " - failed to open the file!!!" << std::endl;
	}
	return result;
}
#pragma endregion
#pragma region Private methods
/// <summary>
/// Calculate symbols information.
/// </summary>
void rANS::SymbolInformation::calculateSymbolsInformation() {
	for (int i = 0; i < 256; i++) {

		_maxEncoderState[i] = ((_renormLow >> _n) << 8) * _frequencies[i];
		_frequencyComplement[i] = (uint16_t)((1 << _n) - _frequencies[i]);
		if (_frequencies[i] < 2) {

			_reciprocalFreq[i] = ~0u;
			_reciprocalShift[i] = 0;
			_bias[i] = _cumulatives[i] + (1 << _n) - 1;
		}
		else {
			uint32_t shift = 0;
			while (_frequencies[i] > (1u << shift)) {
				shift++;
			}

			_reciprocalFreq[i] = (uint32_t)(((1ull << (shift + 31)) + _frequencies[i] - 1) / _frequencies[i]);
			_reciprocalShift[i] = shift - 1;
		}
			_bias[i] = _cumulatives[i];
	}
}
#pragma endregion