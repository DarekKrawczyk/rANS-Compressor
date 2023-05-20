#include "rANSCompressor.hpp"

/// <summary>
/// Default constructor of rANS compressor.
/// </summary>
rANS::rANSCompressor::rANSCompressor() {
	//TODO: Determine what are the _alphabet, _freq, _cumul and _M values.
	_M = 8;
	_alphabet = { 0,1,2 };
	_freq = { 3,3,2 };
	_cumul = { 0,3,6 };
}

/// <summary>
/// Default destructor.
/// </summary>
rANS::rANSCompressor::~rANSCompressor() {

}

/// <summary>
/// Encode given data.
/// </summary>
/// <param name="data">Array of data to encode.</param>
/// <param name="dataSize">Size of array of data.</param>
/// <returns>Returns data in encoded format.</returns>
int rANS::rANSCompressor::encode(int* data, size_t dataSize) {
	_encoderState = 0;
	for (int i = 0; i < dataSize; i++) {
		this->encodeStep(data[i]);
	}
	return _encoderState;
}

/// <summary>
/// Encoding of a single symbol.
/// </summary>
/// <param name="symbol">Symbol to encode.</param>
void rANS::rANSCompressor::encodeStep(int symbol) {
	int blockID = floor(_encoderState / _freq[symbol]);
	int slot = _cumul[symbol] + (_encoderState mod _freq[symbol]);
	int nextState = (blockID * _M) + slot;
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
	int blockID = floor(_encoderState / _M);
	int slot = _encoderState mod _M;
	int symbol = this->findNearestBin(slot);
	int nextState = blockID * _freq[symbol] + slot - _cumul[symbol];
	_encoderState = nextState;
	return symbol;
}

int rANS::rANSCompressor::findNearestBin(int slot) {
	int iterator = 0;
	for (auto cumul : _cumul) {
		if (cumul == slot) {
			return iterator;
		}
		else if (cumul > slot) {
			return iterator - 1;
		}
		iterator++;
	}
	return 0;
}