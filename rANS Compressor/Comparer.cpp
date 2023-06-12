#include "Comparer.hpp"

bool Comparer::compareFiles(std::string one, std::string another)
{
	std::string firstText = "";
	std::string secondText = "";

	std::ifstream file(one);
	if (file.is_open()) {
		char character;
		while (file.get(character)) {
			firstText += character;
		}

		file.close();
		std::ifstream secondFile(another);
		if (secondFile.is_open()) {
			char character;
			while (secondFile.get(character)) {
				secondText += character;
			}

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
