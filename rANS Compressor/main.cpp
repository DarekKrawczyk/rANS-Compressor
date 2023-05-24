#include <iostream>
#include <math.h>
#include "rANSCompressor.hpp"
#include <vector>
#include <list>
#include "string"
#include "SymbolInformation.hpp"

using namespace rANS;

/* Links:
    - https://www.youtube.com/watch?v=6vNBLGKsssA&ab_channel=StanfordResearchTalks
    - https://kedartatwawadi.github.io/post--ANS/
    - https://www.asciitable.com/
*/

int main()
{
    const size_t inputSize = 4;
    int input[inputSize] = { 1, 0, 2, 1 };
    int result = 0;
    //std::cout << "Result: " << result << std::endl;

    //std::cout << "rANS decoding: " << result << std::endl;
    //const size_t size = 4;
    //int results[size] = { 0,0,0,0 };
    //decode(results, size, result);
    //std::cout << "Result: {" << results[0] << "," << results[1] << "," << results[2] << "," << results[3] << "}" << std::endl;
    std::cout << "-------------------- with Classes --------------------\n";
    std::cout << "rANS encoder input = { 1, 0, 2, 1 }\n";
    SymbolInformation testingData("102110211021");
    rANSCompressor compressor;
    int classResult = compressor.encode(testingData);
    std::cout << "Result: " << result << std::endl;
    std::cout << "rANS decoding: " << classResult << std::endl;
    std::list<int> preEncoding;
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
