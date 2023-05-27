#include <iostream>
#include <math.h>
#include "rANSCompressor.hpp"
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include "SymbolInformation.hpp"

using namespace rANS;

/* Links:
    - https://www.youtube.com/watch?v=6vNBLGKsssA&ab_channel=StanfordResearchTalks
    - https://kedartatwawadi.github.io/post--ANS/
    - https://www.asciitable.com/
*/

int main()
{
    std::string message = "102110211021";
    std::string result = "";
    std::cout << "-------------------- with Classes --------------------\n";
    std::cout << "rANS encoder input = { 1, 0, 2, 1 }\n";
    SymbolInformation testingData(message);
    rANSCompressor compressor;
    result = compressor.encode(testingData);
    std::cout << "Result: " << result << std::endl;
    //printf("Result: %s\n", result.c_str());

    std::string decoded = compressor.decode(testingData, result);

    //Save to file
    std::ofstream file("result.txt"); 
    if (file.is_open()) {
        file << result; 
        file.close(); 
        std::cout << "String saved to file successfully." << std::endl;
    }
    else {
        std::cout << "Unable to open the file." << std::endl;
    }


    std::ifstream checkFile("D:\\Dev\\rANS Compressor\\rANS Compressor\\result.txt");
    std::string fileReadData;
    if (checkFile.is_open()) {
        char symbol;

        while (checkFile.get(symbol)) {
            fileReadData += symbol;
        }

        checkFile.close();

        std::cout << "Data read from file: " << fileReadData << std::endl;
    }
    else {
        std::cout << "Unable to open the file." << std::endl;
    }

    if (fileReadData == result) {
        std::cout << "Encoded data and data red from from file is the same - GOOD!\n";
    }
    else {
        std::cout << "Encoded data and data red from from file is NOT the same - BAD!\n";
    }

    //compressor.decode(classResult, preEncoding);
    //compressor.decode(preEncoding);
    //printResults(preEncoding);
    //std::cout << "-------------------- Data input --------------------\n";
    //SymbolInformation* info = new SymbolInformation();
    //info->loadData("D:\\Dev\\rANS Compressor\\Debug\\DATA.txt");
    //info->calculateMetric();
    //info->printData();
    //std::cout << "-------------------- rANSCompressor --------------------\n";
    //SymbolInformation tt(*info);
    //tt.printData();
    //rANSCompressor second(tt);
    //second.printData();
    //rANSCompressor third(info);
    //third.printData();
    //std::cout << "-------------------- Getters test --------------------\n";
    //delete info;
    //uint32_t* buffer = info->getFrequencies();
    //int length = info->getAlphabetSize();
    //delete info;
    //for (int i = 0; i < length; i++) {
    //    std::cout << buffer[i] << std::endl;
    //}
    //delete[] buffer;

    std::cin.get();
}
