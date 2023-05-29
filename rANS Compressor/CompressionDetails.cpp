#include "CompressionDetails.hpp"

rANS::CompressionDetails::CompressionDetails()
{
	_objectSize = -1;
	_operationSpeed = -1;
}

void rANS::CompressionDetails::setOperationTime(std::chrono::duration<double> time)
{
	_operationTime = time;
}

void rANS::CompressionDetails::setObjectSize(size_t size)
{
	_objectSize = size;
}

void rANS::CompressionDetails::calculate()
{
	double speed = _objectSize / _operationTime.count();
	_operationSpeed = speed;
}

double rANS::CompressionDetails::getOperationTime() const
{
	return _operationTime.count();
}

double rANS::CompressionDetails::getOperationSpeed() const
{
	return _operationSpeed;
}

size_t rANS::CompressionDetails::getObjectSize() const
{
	return _objectSize;
}
