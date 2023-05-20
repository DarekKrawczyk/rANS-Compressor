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
*/

//Alphabet -> tablica wszystkich mo¿liwych do zakodowania symboli.
//M -> mianownik prawdopodobieñstwa.
const int M = 8;
const size_t inputSize = 4;
const size_t size = 3;
int Alphabet[size] = { 0, 1, 2 };
//Prob - probability -> tablica przechowuj¹ca prawdopodobieñstwa wyst¹pienia danego symbolu.
float Prob[size] = { 3 / M, 3 / M, 2 / M };
//Freq - frequencies -> tablica zawieraj¹ca liczniki z prawdopodobieñstwa wyst¹pienia danego symbolu.
int Freq[size] = { 3, 3, 2 };
//Cumul -> tablica przechowuj¹ca sumê czêstotliwoœci poprzednio zdefiniowanych symboli.
int Cumul[size] = { 0, 3, 6 };

//This is a function to be called on every step (element of array) of input value.
//It takes as argument previous value of the encoder, and element of an array to be compressed.
int encodeStep(int x, int symbol) {
    
    int blockID = floor(x / Freq[symbol]);
    int slot = Cumul[symbol] + (x%Freq[symbol]);
    int nextState = blockID * M + slot;
    std::cout << "Next state: " << nextState << std::endl;
    return nextState;
}

//Encoder takes an input array.
int encode(int* inputTable, size_t size) {
    //Initial state of the encoder. Always starts with 0.
    int x = 0;
    for (int i = 0; i < size; i++) {
        x = encodeStep(x, inputTable[i]);
    }

    //One feature missing here is converting result to binary form.

    return x;
}

void reverseResult(int* tableResult) {
    int temp[inputSize];
    for (int i = 0; i < inputSize; i++) {
        temp[i] = tableResult[inputSize - i - 1];
    }
    for (int i = 0; i < inputSize; i++) {
        tableResult[i] = temp[i];
    }
}

int findNearestBin(int slot) {
    int index = 0;
    for (int i = 0; i < size; i++) {
        if (Cumul[i] == slot) {
            return i;
        }
        else if (Cumul[i] > slot) {
            index = i - 1;
            return index;
        }
    }
    return 0;
}

int decodeStep(int& previousX) {
    int blockID = floor(previousX / M);
    int slot = previousX % M;
    int symbol = findNearestBin(slot);
    int x = blockID * Freq[symbol] + slot - Cumul[symbol];
    previousX = x;
    return symbol;
}

void decode(int* results, size_t resultSize, int value) {
    int state = value;
    for (int i = 0; i < resultSize; i++) {
        results[i] = decodeStep(state);
    }
    reverseResult(results);
}

void printResults(std::list<int> data) {
    std::string result = "Result = {";
    for (auto symbol : data) {
        result += std::to_string(symbol) + ",";
    }
    result += "}\n";
    std::cout << result;
}

int main()
{
    std::cout << "rANS encoder input = { 1, 0, 2, 1 }\n";
    const size_t inputSize = 4;
    int input[inputSize] = { 1, 0, 2, 1 };
    int result = encode(input, inputSize);
    std::cout << "Result: " << result << std::endl;

    std::cout << "rANS decoding: " << result << std::endl;
    const size_t size = 4;
    int results[size] = { 0,0,0,0 };
    decode(results, size, result);
    std::cout << "Result: {" << results[0] << "," << results[1] << "," << results[2] << "," << results[3] << "}" << std::endl;
    std::cout << "-------------------- with Classes --------------------\n";
    std::cout << "rANS encoder input = { 1, 0, 2, 1 }\n";
    rANSCompressor compressor = rANSCompressor();
    int classResult = compressor.encode(input, inputSize);
    std::cout << "Result: " << result << std::endl;
    std::cout << "rANS decoding: " << classResult << std::endl;
    std::list<int> preEncoding;
    //compressor.decode(classResult, preEncoding);
    compressor.decode(preEncoding);
    printResults(preEncoding);
    std::cout << "-------------------- Data input --------------------\n";
    SymbolInformation info;
    info.loadData("D:\\Dev\\rANS Compressor\\Debug\\DATA.txt");
    std::cin.get();
}
