#include "rANSCompressor.hpp"

/// <summary>
/// Default constructor of rANS compressor.
/// </summary>
rANS::rANSCompressor::rANSCompressor() {
	//TODO: Determine what are the _alphabet, _FRQ, _CDF and _M values.
	_encodedBuffer = "";
}

rANS::rANSCompressor::rANSCompressor(const SymbolInformation &data) {
	_data = data;
	_encodedBuffer = "";
}

rANS::rANSCompressor::rANSCompressor(const SymbolInformation* data) {
	_data = *data;
	_encodedBuffer = "";
}

/// <summary>
/// Default destructor.
/// </summary>
rANS::rANSCompressor::~rANSCompressor() {
}

void rANS::rANSCompressor::write16bits() {
	uint32_t value = _encoderState & _msk;
	char value1 = value >> 8;
	char value2 = value & (0xff);
	_encodedBuffer += value2;
	_encodedBuffer += value1;
}

uint32_t rANS::rANSCompressor::read16bits() {
	uint32_t temp = 0;
	uint8_t value22 = _encodedBuffer[_encodedBuffer.length() - 1];
	temp = (uint32_t)value22;
	_encodedBuffer.pop_back();
	uint8_t value222 = _encodedBuffer[_encodedBuffer.length() - 1];
	uint32_t temp2 = (uint32_t)value222;
	uint32_t result = temp + (temp2 << 8);
	_encodedBuffer.pop_back();


	//char value1 = _encodedBuffer[_encodedBuffer.length() - 1];
	//_encodedBuffer.pop_back();
	//char value2 = 0;
	//if (_encodedBuffer.length() > 0) {
	//	value2 = _encodedBuffer[_encodedBuffer.length() - 1];
	//	_encodedBuffer.pop_back();
	//}
	//uint32_t result = value1 + ((int)value2 << 8);
	return temp;
	//uint32_t value = _encoderState & _msk;
	//char value1 = value >> 8;
	//char value2 = value & (0xff);
	//_encodedBuffer += value1;
	//_encodedBuffer += value2;
}

/// <summary>
/// Encode given data.
/// </summary>
/// <param name="data">Array of data to encode.</param>
/// <param name="dataSize">Size of array of data.</param>
/// <returns>Returns data in encoded format.</returns>
std::string rANS::rANSCompressor::encode(const SymbolInformation& data) {
	_data = data;
	_encodedBuffer = "";
	_encoderState = 1u << 23;
	int size = _data.getBufferSize();
	for (int i = 0; i < size; i++) {
		this->encodeStep(_data.getBuffer(i));
	}

	//Write state of encoder into buffer. _encoderState variable is uint32_t thus 4 bytes.
	char symbol4 = ((_encoderState >> 24) & (0xff));
	_encodedBuffer += symbol4;

	char symbol3 = ((_encoderState >> 16) & (0xff));
	_encodedBuffer += symbol3;

	char symbol2 = ((_encoderState >> 8) & (0xff));
	_encodedBuffer += symbol2;

	char symbol = (_encoderState & (0xff));
		_encodedBuffer += symbol;

	return _encodedBuffer;
}

/// <summary>
/// Encoding of a single symbol.
/// </summary>
/// <param name="symbol">Symbol to encode.</param>
void rANS::rANSCompressor::encodeStep(int symbol) {
	uint32_t freq = _data.getFrequencies(symbol);
	uint32_t cumul = _data.getCumul(symbol);
	uint32_t M = _data.getM();
	uint32_t D = _data.getD();

	//Renormalize
	//if (_encoderState >= (freq << D)) {
	if (_encoderState >= (freq << D)) {
		write16bits();
		_encoderState >>= 16;
	}

	uint32_t blockID = floor(_encoderState / freq) * M;
	uint32_t slot = 14 + (_encoderState mod freq);
	//uint32_t nextState = (blockID * M) + slot;
	uint32_t nextState = blockID + slot + cumul;
	_encoderState = nextState;
}

std::string rANS::rANSCompressor::decode(const SymbolInformation& info, const std::string& encodedData) {
	_data = info;
	_decodedBuffer = "";
	_encodedBuffer = encodedData;
	_decoderState = 0;

	//Initialize decoding - push last elenets of encoded data to decoder state;
	char value = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState = value;
	_encodedBuffer.pop_back();
	char value2 = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState |= (value2 << 8);
	_encodedBuffer.pop_back();
	char value3 = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState |= (value3 << 16);
	_encodedBuffer.pop_back();
	char value4 = _encodedBuffer[_encodedBuffer.length() - 1];
	_decoderState |= (value4 << 24);
	_encodedBuffer.pop_back();


	int size = _data.getBufferSize();
	while(_decoderState != 0) {
		this->decodeStepik();
	}
	return "";
}

void rANS::rANSCompressor::decodeStepik() {
	//Get symbol
	uint32_t mask = (1u << 14) - 1;
	uint32_t bucket = (_decoderState & mask);
	uint32_t symbol = _data._symbols[_decoderState & mask];
	char found = (char)symbol;

	_decodedBuffer.push_back(symbol);

	int blockID = _decoderState >> 14;
	int slot = _decoderState & mask;
	int nextState = blockID * _data.getFrequencies(symbol) + slot - _data.getCumul(symbol);
	_decoderState = nextState;

	if (_decoderState < pow(2, 16)) {
		if (_encodedBuffer.length() > 0) {
			uint32_t bitsToAdd = read16bits();
			uint32_t bitShifted = _decoderState << 16;
			_decoderState = bitShifted + bitsToAdd;
		}
	}
}

/// <summary>
/// Decodes rANS format, based on last encoded value.
/// </summary>
/// <param name="result">Vector of decoded symbols.</param>
void rANS::rANSCompressor::decode(std::list<int>& result) {
	if (_encoderState <= 0) {
		return;
	}
	//DONT USE THS FUNCTION, WHILE LOOP WILL GO FOR INFINITY 
	else {
		result.clear();
		int iteration = 0;
		int symbol = 0;
		//Till there is anything to decode, decode symbol.
		while (_encoderState != 0) {
			symbol = this->decodeStep();
			result.push_front(symbol); //Using push_front() so there is no need to inverse result.
			iteration++;
		}
	}
}

/// <summary>
/// Decodes rANS format based on provided value.
/// </summary>
/// <param name="value">Value to be decoded.</param>
/// <param name="result">Vector of decoded symbols.</param>
void rANS::rANSCompressor::decode(int value, std::list<int>& result) {

}

int rANS::rANSCompressor::decodeStep() {
	int blockID = floor(_encoderState / _data.getM());
	int slot = _encoderState mod _data.getM();
	int symbol = this->findNearestBin(slot);
	int nextState = blockID * _data.getFrequencies(symbol) + slot - _data.getCumul(symbol);
	_encoderState = nextState;
	return symbol;
}

int rANS::rANSCompressor::findNearestBin(int slot) {
	return _data.findNearestBin(slot);
	//int iterator = 0;
	//for (auto cumul : _CDF) {
	//	if (cumul == slot) {
	//		return iterator;
	//	}
	//	else if (cumul > slot) {
	//		return iterator - 1;
	//	}
	//	iterator++;
	//}
	//return 0;
}

void rANS::rANSCompressor::printData() {
	_data.printData();
}