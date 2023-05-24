#include "rANSCompressor.hpp"

/// <summary>
/// Default constructor of rANS compressor.
/// </summary>
rANS::rANSCompressor::rANSCompressor() {
	//TODO: Determine what are the _alphabet, _FRQ, _CDF and _M values.
	_outBufferIterator = 0;
	_outBuffer = new uint8_t[10];
}

rANS::rANSCompressor::rANSCompressor(const SymbolInformation &data) {
	_data = data;
	_outBufferIterator = 0;
	_outBuffer = new uint8_t[10];
}

rANS::rANSCompressor::rANSCompressor(const SymbolInformation* data) {
	_data = *data;
	_outBufferIterator = 0;
	_outBuffer = new uint8_t[10];
}

/// <summary>
/// Default destructor.
/// </summary>
rANS::rANSCompressor::~rANSCompressor() {
	delete[] _outBuffer;
}

void rANS::rANSCompressor::write16bits() {
	uint32_t value = _encoderState & _msk;
	uint8_t value1 = value >> 8;
	uint8_t value2 = value & (0xff);
	_outBuffer[_outBufferIterator++] = value1;
	_outBuffer[_outBufferIterator++] = value2;
}

/// <summary>
/// Encode given data.
/// </summary>
/// <param name="data">Array of data to encode.</param>
/// <param name="dataSize">Size of array of data.</param>
/// <returns>Returns data in encoded format.</returns>
int rANS::rANSCompressor::encode(const SymbolInformation& data) {
	_data = data;
	_encoderState = 0;
	int size = _data.getBufferSize();
	for (int i = 0; i < size; i++) {
		this->encodeStep(_data.getBuffer(i));
	}
	while (_encoderState > 0) {
		_outBuffer[_outBufferIterator++] = _encoderState & (0xff);
		_encoderState >>= 8;
	}
	uint8_t result[10];
	for (int i = 0; i < 10; i++) {
		result[i] = _outBuffer[i];
	}
	return _encoderState;
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
	if (_encoderState >= (freq << D)) {
		write16bits();
		_encoderState >>= 16;
	}

	uint32_t blockID = floor(_encoderState / freq);
	uint32_t slot = cumul + (_encoderState mod freq);
	uint32_t nextState = (blockID * M) + slot;
	_encoderState = nextState;
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