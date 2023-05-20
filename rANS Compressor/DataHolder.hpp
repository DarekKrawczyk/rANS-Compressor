#pragma once
#include <vector>

namespace rANS {
	class DataHolder
	{
	public:
		DataHolder(int* data, size_t dataSize);
		~DataHolder();

	private:
		size_t _dataSize;
		std::vector<int> *_data;
	};
}
