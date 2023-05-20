#include "DataHolder.hpp"

rANS::DataHolder::DataHolder(int* data, size_t dataSize) {
	for (int i = 0; i < dataSize; i++) {
		_data->push_back(data[i]);
	}
	_dataSize = dataSize;
}

rANS::DataHolder::~DataHolder() {
}