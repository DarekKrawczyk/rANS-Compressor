#include "Comparer.hpp"

bool Comparer::compareFiles(std::string one, std::string another)
{
	std::string firstText = "";
	std::string secondText = "";

	std::ifstream file(one, std::ios_base::binary);
	if (file.is_open()) {
		firstText = std::string((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		file.close();
		std::ifstream secondFile(another, std::ios_base::binary);
		if (secondFile.is_open()) {
			secondText = std::string((std::istreambuf_iterator<char>(secondFile)),
				std::istreambuf_iterator<char>());

			secondFile.close();

			if (firstText == secondText) {
				return true;
			}
			else {
				return false;
			}

		}
		else {
			std::cout << another << " - failed to open the file!!!" << std::endl;
		}
	}
	else {
		std::cout << one << " - failed to open the file!!!" << std::endl;
	}
	return false;
}

bool Comparer::compareBuffers()
{
	return false;
}
