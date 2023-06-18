#pragma once
#include <chrono>

namespace rANS {
	class CompressionDetails {
	public:
		CompressionDetails();

		void setOperationTime(std::chrono::duration<double> time);
		void setObjectSize(size_t size);
		void calculate();

		double getOperationTime() const;
		double getOperationSpeed() const;
		size_t getObjectSize() const;

		static double CalculateAvg(double* tab, size_t size);

	private:
		std::chrono::duration<double> _operationTime;
		double _operationSpeed;
		size_t _objectSize;
	};
}
