#include "rANSCompressor.hpp"

/// <summary>
/// Default constructor of rANS compressor.
/// </summary>
rANS::rANSCompressor::rANSCompressor() {
	_encodedBuffer = "";
}

/// <summary>
/// rANSCompressor constructor initializes symbol information data.
/// </summary>
/// <param name="info">Symbols information data.</param>
rANS::rANSCompressor::rANSCompressor(const SymbolInformation & info) {
	_symbolInformation = info;
	_encodedBuffer = "";
}

/// <summary>
/// rANSCompressor constructor initializes symbol information data.
/// </summary>
/// <param name="info">Symbols information data.</param>
rANS::rANSCompressor::rANSCompressor(const SymbolInformation* info) {
	_symbolInformation = *info;
	_encodedBuffer = "";
}

/// <summary>
/// Default destructor.
/// </summary>
rANS::rANSCompressor::~rANSCompressor() {
}

/// <summary>
/// Encode file with rANS compressor. Methods creates "output" file with "results.txt" file that is an encoded message.
/// </summary>
/// <param name="path">Path of fileto be encoded.</param>
/// <returns>0 - operation successful, 1 - file not found, 2 - error</returns>
uint32_t rANS::rANSCompressor::encodeFile(std::string pathOfFileToEncode)
{
	//Create buffer
	std::shared_ptr<std::string> dataBuffer = std::make_shared<std::string>();

	//Open to be encoded and calculate symbol information.
	SymbolInformation symbolInfo;
	dataBuffer = symbolInfo.loadDataFromFile(pathOfFileToEncode);
	symbolInfo.toFile();

	//Create folder and file that is result encoded message.
	_outputFile.open("encoded.txt");

	//Load symbol information and reset all of the buffers.
	_symbolInformation = symbolInfo;
	_decoderState = 0;
	_decodedBuffer = "";
	_encodedBuffer = "";
	_encoderState = _symbolInformation.getLowRenormBoundary();		//Starting with low boundary value.

	//Start measuring time.
	std::chrono::steady_clock::time_point beginTime = std::chrono::high_resolution_clock::now();

	//For each symbol in buffer perform encoding operation.
	size_t size = _symbolInformation.getBufferSize();
	for (size_t i = 0; i < size; i++) {
		this->encodeStep(dataBuffer->at(i), false);
	}

	//Write state of encoder into buffer. _encoderState variable is uint32_t thus 4 bytes.
	uint8_t lastSymbol1 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	uint8_t lastSymbol2 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	uint8_t lastSymbol3 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	uint8_t lastSymbol4 = (_encoderState & (0xff));

	this->write8bits(lastSymbol4);
	this->write8bits(lastSymbol3);
	this->write8bits(lastSymbol2);
	this->write8bits(lastSymbol1);

	//Finish measuring time.
	std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	//Close file.
	_outputFile.close();

	//Get file path
	size_t index = pathOfFileToEncode.find_last_of('\\');
	std::string path = pathOfFileToEncode.substr(0, pathOfFileToEncode.length() - (pathOfFileToEncode.length() - index)) + "\\encoded.txt";
	
	//Get file size.
	std::filesystem::path encodedFile{path};
	size_t fileSize = std::filesystem::file_size(encodedFile);

	//Fill information about encoding process.
	_encodingDetails.setOperationTime(endTime - beginTime);
	_encodingDetails.setObjectSize(fileSize);
	_encodingDetails.calculate();

	return 0;
}

/// <summary>
/// Encode given data.
/// </summary>
/// <param name="info">Object with data to encode with necessary information about symbols.</param>
/// <returns>Returns data in encoded format.</returns>
std::string rANS::rANSCompressor::encode(const std::string& dataBuffer, const SymbolInformation& info) {
	//Load symbol information and reset all of the buffers.
	_symbolInformation = info;
	_decoderState = 0;
	_decodedBuffer = "";
	_encodedBuffer = "";
	_encoderState = _symbolInformation.getLowRenormBoundary();		//Starting with low boundary value.

	//Start measuring time.
	std::chrono::steady_clock::time_point beginTime = std::chrono::high_resolution_clock::now();

	//For each symbol in buffer perform encoding operation.
	size_t size = _symbolInformation.getBufferSize();
	for (size_t i = 0; i < size; i++) {
		this->encodeStep(dataBuffer[i]);
	}

	//Write state of encoder into buffer. _encoderState variable is uint32_t thus 4 bytes.
	uint8_t lastSymbol1 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	uint8_t lastSymbol2 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	uint8_t lastSymbol3 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	uint8_t lastSymbol4 = (_encoderState & (0xff));

	_encodedBuffer += lastSymbol4;
	_encodedBuffer += lastSymbol3;
	_encodedBuffer += lastSymbol2;
	_encodedBuffer += lastSymbol1;

	//Finish measuring time.
	std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();
	
	//Fill information about encoding process.
	_encodingDetails.setOperationTime(endTime - beginTime);
	_encodingDetails.setObjectSize(_encodedBuffer.size());
	_encodingDetails.calculate();

	return _encodedBuffer;
}

/// <summary>
/// Encode a single symbol.
/// </summary>
/// <param name="symbol">Symbol to encode.</param>
void rANS::rANSCompressor::encodeStep(uint32_t symbol, bool toBuffer) {
#if Implementation == 0
    uint32_t tempEncoderState = _encoderState;
    uint32_t maxEncoderState = _symbolInformation.getMaxEncoderState(symbol);

	//Renormalization.
    if (tempEncoderState >= maxEncoderState) {
        do {
			//Write last 8 bites into encoded buffer.
            uint8_t last8Bits = (uint8_t)(tempEncoderState & 0xff);
			if (toBuffer == true) {
				_encodedBuffer += last8Bits;
			}
			else {
				this->write8bits(last8Bits);
			}
			tempEncoderState >>= 8;
        } while (tempEncoderState >= maxEncoderState);
    }

	//Calculating new encoder state.
    uint32_t q = (uint32_t)(((uint64_t)tempEncoderState * _symbolInformation.getReciprocalFreq(symbol)) >> 32) >> _symbolInformation.getReciprocalShift(symbol);
	_encoderState = tempEncoderState + _symbolInformation.getBias(symbol) + q * _symbolInformation.getFreqComplement(symbol);
#endif
#if Implementation == 1
	uint32_t freq = _data.getFrequency(symbol);
	uint32_t cumul = _data.getCumulative(symbol);
	uint32_t M = _data.getCumulative(ALPHABET_SIZE);
	uint32_t D = _data.getD();

	//Renormalize
	//if (_encoderState >= (freq << D)) {
	//if (_encoderState >= (freq << (32-14))) {
	uint32_t codix = (freq << (32 - 14));
	if (_encoderState >= codix) {
		uint8_t last8Bits = (uint8_t)(_encoderState & 0xff);
		_encodedBuffer += last8Bits;
		_encoderState >>= 8;
	}

	uint32_t blockID = floor(_encoderState / freq) * M;
	uint32_t slot = (_encoderState mod freq);
	//uint32_t nextState = (blockID * M) + slot;
	uint32_t nextState = blockID + slot + cumul;
	_encoderState = nextState;
#endif 
}

uint32_t rANS::rANSCompressor::decodeFile(std::string pathOfFileToDecode, std::string pathOfFileWithSymbolInfo)
{
	//Loading symbol information.
	SymbolInformation info;
	info.loadSymbolInfoFromFile(pathOfFileWithSymbolInfo);

	//Opening encoded file and reading its contence.
	std::string encodedData = "";
	std::ifstream file(pathOfFileToDecode);
	if (file.is_open()) {
		//std::cout << pathOfFileToDecode << " - file opened successfully!" << std::endl;

		//Read symbols from file.
		encodedData = "";
		char character;
		while (file.get(character)) {
			encodedData += character;
		}

		file.close();

		std::ifstream t("encoded.txt");
		std::stringstream bufik;
		bufik << t.rdbuf();
		std::string sd = bufik.str();
	}
	else {
		std::cout << pathOfFileToDecode << " - failed to open the file!!!" << std::endl;
	}

	//Load symbol information, encoded data and reset all of the buffers.
	_symbolInformation = info;
	_encodedBuffer = encodedData;
	_encoderState = 0;
	_decodedBuffer = "";
	_decoderState = 0;

	//Start measuring time.
	std::chrono::steady_clock::time_point beginTime = std::chrono::high_resolution_clock::now();

	//Initialize decoding - push last symbols of encoded data to decoder state;
	uint8_t value = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState = value;
	_encodedBuffer.pop_back();
	uint8_t value2 = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState |= (value2 << 8);
	_encodedBuffer.pop_back();
	uint8_t value3 = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState |= (value3 << 16);
	_encodedBuffer.pop_back();
	uint8_t value4 = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState |= (value4 << 24);
	_encodedBuffer.pop_back();

	//For each symbol in buffer perform decoding operation.
	size_t size = _symbolInformation.getBufferSize();
	for (size_t i = 0; i < size; i++) {
		this->decodeStep();
	}

	//Reverse decoded buffer because data is decoded in reverse.
	std::reverse(_decodedBuffer.begin(), _decodedBuffer.end());

	//Opening result file.
	_outputFile.open("decoded.txt");

	//Save result to file.
	_outputFile << _decodedBuffer;

	//Closing result file.
	_outputFile.close();

	//Finish measuring time.
	std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	//Get file path
	size_t index = pathOfFileToDecode.find_last_of('\\');
	std::string path = pathOfFileToDecode.substr(0, pathOfFileToDecode.length() - (pathOfFileToDecode.length() - index)) + "\\decoded.txt";

	//Get file size.
	std::filesystem::path decodedFile{path};
	size_t fileSize = std::filesystem::file_size(decodedFile);

	//Fill information about encoding process.
	_decodingDetails.setOperationTime(endTime - beginTime);
	_decodingDetails.setObjectSize(fileSize);
	_decodingDetails.calculate();

	return 0;
}

/// <summary>
/// rANS decoder.
/// </summary>
/// <param name="info">Information about encoded data.</param>
/// <param name="encodedData">Encoded data.</param>
/// <returns>Decoded data.</returns>
std::string rANS::rANSCompressor::decode(const SymbolInformation& info, const std::string& encodedData) {
	//Load symbol information, encoded data and reset all of the buffers.
	_symbolInformation = info;
	_encodedBuffer = encodedData;
	_encoderState = 0;
	_decodedBuffer = "";
	_decoderState = 0;

	//Start measuring time.
	std::chrono::steady_clock::time_point beginTime = std::chrono::high_resolution_clock::now();

	//Initialize decoding - push last symbols of encoded data to decoder state;
	uint8_t value = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState = value;
	_encodedBuffer.pop_back();
	uint8_t value2 = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState |= (value2 << 8);
	_encodedBuffer.pop_back();
	uint8_t value3 = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState |= (value3 << 16);
	_encodedBuffer.pop_back();
	uint8_t value4 = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState |= (value4 << 24);
	_encodedBuffer.pop_back();

	//For each symbol in buffer perform decoding operation.
	size_t size = _symbolInformation.getBufferSize();
	for (size_t i = 0; i < size; i++) {
		this->decodeStep();
	}

	//Reverse decoded buffer because data is decoded in reverse.
	std::reverse(_decodedBuffer.begin(), _decodedBuffer.end());

	//Finish measuring time.
	std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	//Fill information about decoding process.
	_decodingDetails.setOperationTime(endTime - beginTime);
	_decodingDetails.setObjectSize(_decodedBuffer.size());
	_decodingDetails.calculate();

	return _decodedBuffer;
}

/// <summary>
/// Decode single symbol.
/// </summary>
void rANS::rANSCompressor::decodeStep() {
	//Calculate mask.
	uint32_t mask = _symbolInformation.getMask();

	//Based on calculated mask decode symbol and save it into buffer.
	uint32_t symbol = _symbolInformation.getSymbol(_decoderState & mask);
	_decodedBuffer.push_back(symbol);

	//After decoding symbol, its time to calculate next _decoderState value. 
	uint32_t blockID = _decoderState >> _symbolInformation.getN();
	uint32_t slot = _decoderState & mask;
	uint32_t nextState = blockID * _symbolInformation.getFrequency(symbol) + slot - _symbolInformation.getCumulative(symbol);
	_decoderState = nextState;

	//Renormalization.
	uint32_t boundary = _symbolInformation.getLowRenormBoundary();
	try {
		if (_decoderState < boundary) {
			do {
				uint8_t sign = _encodedBuffer[_encodedBuffer.length() - 1];
				_encodedBuffer.pop_back();
				_decoderState = (_decoderState << 8) | sign;
			} while (_decoderState < boundary);
		}
	}
	catch (int counter) {
		std::cout << "error\n";
	}
}

/// <summary>
/// Print information about symbol infoormation.
/// </summary>
void rANS::rANSCompressor::printData() {
	_symbolInformation.printData();
}

rANS::CompressionDetails rANS::rANSCompressor::getEncodingDetails() const
{
	return _encodingDetails;
}

rANS::CompressionDetails rANS::rANSCompressor::getDecodingDetails() const
{
	return _decodingDetails;
}

/// <summary>
/// Read last byte from _encodedBuffer and remove it from this buffer.
/// </summary>
/// <returns>Last byte from encoded buffer.</returns>
uint8_t rANS::rANSCompressor::read8bits() {
	uint8_t last8bits = _encodedBuffer[_encodedBuffer.length() - 1];
	_encodedBuffer.pop_back();
	return last8bits;
}

void rANS::rANSCompressor::write8bits(uint8_t buffer)
{
	_outputFile << buffer;
}
