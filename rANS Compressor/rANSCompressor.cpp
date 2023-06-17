#include "rANSCompressor.hpp"

/// <summary>
/// Default constructor of rANS compressor.
/// </summary>
rANS::rANSCompressor::rANSCompressor() {
	folder = OUTPUT_FOLDER;
	_encodedBuffer = std::make_shared<std::list<uint8_t>>();
	_decodedBuffer = std::make_shared<std::list<uint8_t>>();
}

/// <summary>
/// rANSCompressor constructor, initializes symbol information data.
/// </summary>
/// <param name="info">Symbols information data.</param>
rANS::rANSCompressor::rANSCompressor(const SymbolInformation& info) {
	folder = OUTPUT_FOLDER;
	_symbolInformation = info;
	_encodedBuffer = std::make_shared<std::list<uint8_t>>();
	_decodedBuffer = std::make_shared<std::list<uint8_t>>();
}

/// <summary>
/// rANSCompressor constructor, initializes symbol information data.
/// </summary>
/// <param name="info">Symbols information data.</param>
rANS::rANSCompressor::rANSCompressor(const SymbolInformation* info) {
	folder = OUTPUT_FOLDER;
	_symbolInformation = *info;
	_encodedBuffer = std::make_shared<std::list<uint8_t>>();
	_decodedBuffer = std::make_shared<std::list<uint8_t>>();
}

/// <summary>
/// Default destructor.
/// </summary>
rANS::rANSCompressor::~rANSCompressor() {
}

/// <summary>
/// Encode a single symbol.
/// </summary>
/// <param name="symbol">Symbol to encode.</param>
void rANS::rANSCompressor::encodeStep(uint8_t symbol) {
	uint32_t tempEncoderState = _encoderState;
	uint32_t maxEncoderState = _symbolInformation.getMaxEncoderState(symbol);

	//Renormalization.
	if (tempEncoderState >= maxEncoderState) {
		do {
			//Get last 8 bits.
			uint8_t last8Bits = (uint8_t)(tempEncoderState & 0xff);

			//Write them info encoded buffer and shift current encoder state by 8.
			_encodedBuffer->push_back(last8Bits);
			tempEncoderState >>= 8;
		} while (tempEncoderState >= maxEncoderState);
	}

	// Calculate new encoder state.
	uint32_t reciprocalFreq = _symbolInformation.getReciprocalFreq(symbol);
	uint64_t scaledState = ((uint64_t)tempEncoderState * reciprocalFreq) >> 32;
	uint64_t reciprocalShift = _symbolInformation.getReciprocalShift(symbol);
	uint32_t output = (uint32_t)(scaledState >> reciprocalShift);

	uint32_t transformedValue = output * _symbolInformation.getFreqComplement(symbol);
	uint32_t incrementedState = tempEncoderState + _symbolInformation.getBias(symbol);

	_encoderState = incrementedState + transformedValue;

#if STATE_LOGGER == 1
	_outputEncodingState << _encoderState << '\n';
#endif
}

/// <summary>
/// Encode given data.
/// </summary>
/// <param name="dataBuffer">Data buffer to encode.</param>
/// <param name="info">Information about probided data buffer.</param>
/// <returns>Shared pointer to output buffer.</returns>
std::shared_ptr<std::list<uint8_t>> rANS::rANSCompressor::encode(const std::shared_ptr<std::list<uint8_t>>& dataBuffer, const SymbolInformation& info) {
	//Load symbol information and reset all of the buffers.
	_symbolInformation = info;
	_decoderState = 0;
	_encoderState = _symbolInformation.getLowRenormBoundary();

	//Start measuring time.
	std::chrono::steady_clock::time_point beginTime = std::chrono::high_resolution_clock::now();

#if STATE_LOGGER == 1
	//Create folder and file that is result encoded message.
	_outputEncodingState.open(folder + "encoderState.txt", std::ofstream::binary);
	_outputEncodingState << _encoderState << '\n';
#endif

	//For each symbol in buffer perform encoding operation.
	for (std::list<uint8_t>::iterator it = dataBuffer->begin(); it != dataBuffer->end(); ++it) {
		uint8_t sign = *it;
		this->encodeStep(sign);
	}

	//Write state of encoder into buffer. _encoderState variable is uint32_t thus 4 bytes.
	uint32_t state1, state2, state3;
	uint8_t lastSymbol1 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	state1 = _encoderState;
	uint8_t lastSymbol2 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	state2 = _encoderState;
	uint8_t lastSymbol3 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	state3 = _encoderState;
	uint8_t lastSymbol4 = (_encoderState & (0xff));

#if STATE_LOGGER == 1
	_outputEncodingState << state1 << '\n';
	_outputEncodingState << state2 << '\n';
	_outputEncodingState << state3 << '\n';
	_outputEncodingState.close();
#endif

	//Write value of encoder state to buffer.
	_encodedBuffer->push_back(lastSymbol1);
	_encodedBuffer->push_back(lastSymbol2);
	_encodedBuffer->push_back(lastSymbol3);
	_encodedBuffer->push_back(lastSymbol4);

	//Finish measuring time.
	std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	//Fill information about encoding process.
	_encodingDetails.setOperationTime(endTime - beginTime);
	_encodingDetails.setObjectSize(_encodedBuffer->size());
	_encodingDetails.calculate();

	return _encodedBuffer;
}

/// <summary>
/// Encode file with rANS compressor. Output file is generated in /data folder.
/// </summary>
/// <param name="encodePath">Path of file to be encoded.</param>
/// <returns>0 - Operation successful; 1 - Failed to load a file</returns>
uint32_t rANS::rANSCompressor::encodeFile(std::string encodePath)
{
	//Create buffer
	std::shared_ptr<std::list<uint8_t>> dataBuffer = std::make_shared<std::list<uint8_t>>();

	//Open to be encoded and calculate symbol information.
	SymbolInformation symbolInfo;
	dataBuffer = symbolInfo.loadDataFromFile(encodePath);

	//Check if failed to load.
	if (dataBuffer == nullptr) {
		return -1;
	}
	symbolInfo.toFile();

	//Load symbol information and reset all of the buffers.
	_symbolInformation = symbolInfo;
	_decoderState = 0;
	_encoderState = _symbolInformation.getLowRenormBoundary();		//Starting with low boundary value.

#if STATE_LOGGER == 1
	_outputEncodingState.open(folder + "encoderState.txt", std::ofstream::binary);
	_outputEncodingState << _encoderState << '\n';
#endif

	//Start measuring time.
	std::chrono::steady_clock::time_point beginTime = std::chrono::high_resolution_clock::now();

	//For each symbol in buffer perform encoding operation.
	for (std::list<uint8_t>::iterator it = dataBuffer->begin(); it != dataBuffer->end(); ++it) {
		uint8_t sign = *it;
		this->encodeStep(sign);
	}

	//Write state of encoder into buffer. _encoderState variable is uint32_t thus 4 bytes.
	uint32_t state1, state2, state3;
	uint8_t lastSymbol1 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	state1 = _encoderState;
	uint8_t lastSymbol2 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	state2 = _encoderState;
	uint8_t lastSymbol3 = (_encoderState & (0xff));
	_encoderState = (_encoderState >> 8);
	state3 = _encoderState;
	uint8_t lastSymbol4 = (_encoderState & (0xff));

	//Write value of encoder state info buffer.
	_encodedBuffer->push_back(lastSymbol4);
	_encodedBuffer->push_back(lastSymbol3);
	_encodedBuffer->push_back(lastSymbol2);
	_encodedBuffer->push_back(lastSymbol1);

#if STATE_LOGGER == 1
	_outputEncodingState << state1 << '\n';
	_outputEncodingState << state2 << '\n';
	_outputEncodingState << state3 << '\n';
	_outputEncodingState.close();
#endif

	//Finish measuring time.
	std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	//Create folder and file that with encoded message.
	_outputFile.open(folder + "encoded", std::ofstream::binary);
	for (const auto& value : *_encodedBuffer) {
		_outputFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}
	_outputFile.close();

	//Get file path
	size_t index = encodePath.find_last_of('\\');
	std::string path = encodePath.substr(0, encodePath.length() - (encodePath.length() - index)) + "\\encoded";

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
/// Decode single symbol.
/// </summary>
void rANS::rANSCompressor::decodeStep() {
	//Calculate mask slot of symbol.
	uint32_t mask = _symbolInformation.getMask();
	uint32_t slot = _decoderState & mask;

	//Based on calculated mask decode symbol and save it into buffer.
	uint8_t symbol = _symbolInformation.getSymbol(_decoderState & mask);
	_decodedBuffer->push_back(symbol);

	//After decoding symbol, its time to calculate next _decoderState value. 
	uint32_t blockID = _decoderState >> _symbolInformation.getN();
	uint32_t nextState = blockID * _symbolInformation.getFrequency(symbol) + slot - _symbolInformation.getCumulative(symbol);
	_decoderState = nextState;

	//Renormalization.
	uint32_t boundary = _symbolInformation.getLowRenormBoundary();
	if (_decoderState < boundary) {
		do {
			uint8_t sign = _encodedBuffer->back();
			_encodedBuffer->pop_back();
			_decoderState = (_decoderState << 8) | sign;
		} while (_decoderState < boundary);
	}
#if STATE_LOGGER == 1
	_outputDecodingState << _decoderState << '\n';
#endif
}

/// <summary>
/// Decode given data.
/// </summary>
/// <param name="info">Information about encoded data.</param>
/// <param name="encodedData">Encoded data.</param>
/// <returns>Shared pointer to decoded data buffer.</returns>
std::shared_ptr<std::list<uint8_t>> rANS::rANSCompressor::decode(const SymbolInformation& info, const std::shared_ptr<std::list<uint8_t>>& encodedData) {
	//Load symbol information, encoded data and reset all of the buffers.
	_symbolInformation = info;
	_encodedBuffer = encodedData;
	_encoderState = 0;
	_decoderState = 0;

	//Start measuring time.
	std::chrono::steady_clock::time_point beginTime = std::chrono::high_resolution_clock::now();

	//Initialize decoding - push last symbols of encoded data to decoder state;
	uint32_t deState1, deState2, deState3, deState4;

	uint8_t value = _encodedBuffer->back();
	_decoderState = value;
	deState1 = _decoderState;
	_encodedBuffer->pop_back();

	uint8_t value2 = _encodedBuffer->back();
	_decoderState = (_decoderState << 8) + value2;
	deState2 = _decoderState;
	_encodedBuffer->pop_back();

	uint8_t value3 = _encodedBuffer->back();
	_decoderState = (_decoderState << 8) | value3;
	deState3 = _decoderState;
	_encodedBuffer->pop_back();

	uint8_t value4 = _encodedBuffer->back();
	_decoderState = (_decoderState << 8) | value4;
	deState4 = _decoderState;
	_encodedBuffer->pop_back();

#if STATE_LOGGER == 1
	_outputDecodingState.open(folder + "decoderState.txt", std::ofstream::binary);
	_outputDecodingState << deState1 << '\n';
	_outputDecodingState << deState2 << '\n';
	_outputDecodingState << deState3 << '\n';
	_outputDecodingState << deState4 << '\n';
#endif

	//For each symbol in buffer perform decoding operation.
	size_t size = _symbolInformation.getBufferSize();
	for (size_t i = 0; i < size; i++) {
		this->decodeStep();
	}

#if STATE_LOGGER == 1
	_outputDecodingState.close();
#endif

	//Reverse decoded buffer because data is decoded in reverse.
	_decodedBuffer->reverse();

	//Finish measuring time.
	std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	//Fill information about decoding process.
	_decodingDetails.setOperationTime(endTime - beginTime);
	_decodingDetails.setObjectSize(_decodedBuffer->size());
	_decodingDetails.calculate();

	return _decodedBuffer;
}

/// <summary>
/// Decode file. Output file is generated in /data folder.
/// </summary>
/// <param name="decodePath">Path of file to decode.</param>
/// <param name="symbolInfoPath">Path of file with symbol informations.</param>
/// <returns>0 - Operation successful; -1 - Failed to load a encoded file; -2 - Failed to load a file with symbol information.</returns>
uint32_t rANS::rANSCompressor::decodeFile(std::string decodePath, std::string symbolInfoPath)
{
	//Loading symbol information.
	SymbolInformation info;
	bool loaded = info.loadSymbolInfoFromFile(symbolInfoPath);
	if (loaded == false) {
		return -2;
	}

	//Load symbol information, encoded data and reset all of the buffers.
	_encodedBuffer = SymbolInformation::LoadBuffer(decodePath);
	if (_encodedBuffer == nullptr) {
		return -1;
	}
	_symbolInformation = info;
	_encoderState = 0;
	_decoderState = 0;

	//Start measuring time.
	std::chrono::steady_clock::time_point beginTime = std::chrono::high_resolution_clock::now();

	//Initialize decoding - push last symbols of encoded data to decoder state;
	uint32_t deState1, deState2, deState3, deState4;

	uint8_t value1 = _encodedBuffer->back();
	_decoderState = value1;
	deState4 = _decoderState;
	_encodedBuffer->pop_back();

	uint8_t value2 = _encodedBuffer->back();
	_decoderState |= (value2 << 8);
	deState3 = _decoderState;
	_encodedBuffer->pop_back();

	uint8_t value3 = _encodedBuffer->back();
	_decoderState |= (value3 << 16);
	deState2 = _decoderState;
	_encodedBuffer->pop_back();

	uint8_t value4 = _encodedBuffer->back();
	_decoderState |= (value4 << 24);
	deState1 = _decoderState;
	_encodedBuffer->pop_back();

#if STATE_LOGGER == 1
	_outputDecodingState.open(folder + "decoderState.txt", std::ofstream::binary);
	_outputDecodingState << deState1 << '\n';
	_outputDecodingState << deState2 << '\n';
	_outputDecodingState << deState3 << '\n';
	_outputDecodingState << deState4 << '\n';
#endif

	//For each symbol in buffer perform decoding operation.
	size_t size = _symbolInformation.getBufferSize();
	for (size_t i = 0; i < size; i++) {
		this->decodeStep();
	}

#if STATE_LOGGER == 1
	_outputDecodingState.close();
#endif

	//Reverse decoded buffer because data is decoded in reverse.
	_decodedBuffer->reverse();

	//Create folder and file that with decoded message.
	_outputFile.open(folder + "decoded", std::ofstream::binary);
	for (const auto& value : *_decodedBuffer) {
		_outputFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}
	_outputFile.close();

	//Finish measuring time.
	std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	//Get file path
	size_t index = decodePath.find_last_of('\\');
	std::string path = decodePath.substr(0, decodePath.length() - (decodePath.length() - index)) + "\\decoded";

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
/// Print information about symbol infoormation.
/// </summary>
void rANS::rANSCompressor::printData() {
	_symbolInformation.printData();
}

/// <summary>
/// Encoding details accessor.
/// </summary>
/// <returns>Encoding data.</returns>
rANS::CompressionDetails rANS::rANSCompressor::getEncodingDetails() const
{
	return _encodingDetails;
}

/// <summary>
/// Decoding details accessor.
/// </summary>
/// <returns>Decoding data.</returns>
rANS::CompressionDetails rANS::rANSCompressor::getDecodingDetails() const
{
	return _decodingDetails;
}

/// <summary>
/// Read last byte from _encodedBuffer and remove it from this buffer.
/// </summary>
/// <returns>Last byte from encoded buffer.</returns>
uint8_t rANS::rANSCompressor::read8bits() {
	uint8_t last8bits = _encodedBuffer->back();
	_encodedBuffer->pop_back();
	return last8bits;
}
