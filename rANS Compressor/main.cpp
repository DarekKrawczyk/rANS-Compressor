#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <math.h>
#include "SymbolInformation.hpp"
#include "rANSCompressor.hpp"

using namespace rANS;

/* Links:
    - https://www.youtube.com/watch?v=6vNBLGKsssA&ab_channel=StanfordResearchTalks
    - https://kedartatwawadi.github.io/post--ANS/
    - https://www.asciitable.com/
*/

int main()
{
    //std::string message = "102110211021";
    //std::string message = "Ala ma kota a kot ma ale!";
    //std::string message = "test";
    //std::string message = "testtesttest";

    std::string message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    std::string encodedMessage = "";
    std::string decodedMessage = "";

    SymbolInformation testingData(message);
    rANSCompressor compressor;

    encodedMessage = compressor.encode(testingData);
    decodedMessage = compressor.decode(testingData, encodedMessage);

    std::cout << "------------------------- Message -------------------------\n";

    //std::cout << "Input buffer: " << message << std::endl;
    //std::cout<<std::endl;
    //std::cout<<std::endl;

    //std::cout << "Encoded buffer: " << encodedMessage << std::endl;
    //std::cout << std::endl;
    //std::cout << std::endl;

    //std::cout << "Decoded buffer: " << decodedMessage << std::endl;
    //std::cout << std::endl;
    //std::cout << std::endl;

    if (decodedMessage == message) {
        std::cout << "Encoded and decoded succesfully!\n";
    }
    else {
        std::cout << "Something went wrong!\n";
    }
    
    CompressionDetails messageEncodingDetails;
    messageEncodingDetails = compressor.getEncodingDetails();
    std::cout << "Message encoding time: " << messageEncodingDetails.getOperationTime() << "[s]" << std::endl;
    std::cout << "Message encoding speed: " << messageEncodingDetails.getOperationSpeed() << "[byte/s]" << std::endl;
    std::cout << "Message size after encoding: " << messageEncodingDetails.getObjectSize() << "[byte]" << std::endl;
    std::cout << std::endl;

    CompressionDetails messageDecodingDetails;
    messageDecodingDetails = compressor.getDecodingDetails();
    std::cout << "Message decoding time: " << messageDecodingDetails.getOperationTime() << "[s]" << std::endl;
    std::cout << "Message decoding speed: " << messageDecodingDetails.getOperationSpeed() << "[byte/s]" << std::endl;
    std::cout << "Message size after decoding: " << messageDecodingDetails.getObjectSize() << "[byte]" << std::endl;
    std::cout << std::endl;

    double nom = messageDecodingDetails.getObjectSize();
    double de = messageEncodingDetails.getObjectSize();
    double messageCR = nom/de;
    std::cout << "Message compression ratio: " << messageCR << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "------------------------- From file -------------------------\n";

    std::string path = "D:\\Dev\\rANS Compressor\\rANS Compressor\\result.txt";
    std::string fileNormal = "";
    std::string encodedFile = "";
    std::string decodedFile = "";
    SymbolInformation infoFromFile;
    infoFromFile.loadDataFromFile(path);
    fileNormal = infoFromFile.getBuffer();
    rANSCompressor encoder;
    encodedFile = encoder.encode(infoFromFile);
    decodedFile = encoder.decode(infoFromFile, encodedFile);

    //std::cout << "Input buffer: " << fileNormal << std::endl;
    //std::cout << std::endl;
    //std::cout << std::endl;

    //std::cout << "Encoded buffer: " << encodedMessage << std::endl;
    //std::cout << std::endl;
    //std::cout << std::endl;

    //std::cout << "Decoded buffer: " << decodedFile << std::endl;
    //std::cout << std::endl;
    //std::cout << std::endl;

    if (decodedFile == fileNormal) {
        std::cout << "Encoded and decoded succesfully!\n";
    }
    else {
        std::cout << "Something went wrong!\n";
    }

    CompressionDetails fileEncodingDetails;
    fileEncodingDetails = encoder.getEncodingDetails();
    std::cout << "File encoding time: " << fileEncodingDetails.getOperationTime() << "[s]" << std::endl;
    std::cout << "File encoding speed: " << fileEncodingDetails.getOperationSpeed() << "[byte/s]" << std::endl;
    std::cout << "File size after encoding: " << fileEncodingDetails.getObjectSize() << "[byte]" << std::endl;
    std::cout << std::endl;

    CompressionDetails fileDecodingDetails;
    fileDecodingDetails = encoder.getDecodingDetails();
    std::cout << "File decoding time: " << fileDecodingDetails.getOperationTime() << "[s]" << std::endl;
    std::cout << "File decoding speed: " << fileDecodingDetails.getOperationSpeed() << "[byte/s]" << std::endl;
    std::cout << "File size after decoding: " << fileDecodingDetails.getObjectSize() << "[byte]" << std::endl;
    std::cout << std::endl;

    double nomFile = fileDecodingDetails.getObjectSize();
    double deFile = fileEncodingDetails.getObjectSize();
    double fileCR = nomFile / deFile;
    std::cout << "File compression ratio: " << fileCR << std::endl;

    std::cin.get();
}
