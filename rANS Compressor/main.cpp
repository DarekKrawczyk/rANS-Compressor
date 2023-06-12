﻿#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <math.h>
#include <filesystem>
#include "SymbolInformation.hpp"
#include "rANSCompressor.hpp"
#include "Comparer.hpp"

#define rANS_test_FILE_TO_CONSOLE 0
#define rANS_test_FILE 0
#define rANS_test_MESSAGE 1

using namespace rANS;

/* Links:
    - https://www.youtube.com/watch?v=6vNBLGKsssA&ab_channel=StanfordResearchTalks
    - https://kedartatwawadi.github.io/post--ANS/
    - https://www.asciitable.com/
*/

int main()
{
    std::string PATH = "D:\\Dev\\rANS Compressor\\rANS Compressor\\";

#if rANS_test_FILE == 1

    std::shared_ptr<std::string> buffer;
    // ---------- File encoding and decoding. ----------
    std::string inputFilename = "data.txt";

    std::cout << "-------------------- Encoding and decoding file --------------------\n\n";

    SymbolInformation testingData;
    buffer = testingData.loadDataFromFile(PATH + inputFilename);
    testingData.toFile();

    SymbolInformation info;
    info.loadSymbolInfoFromFile(PATH + "symbolInformations.txt");

    bool same = testingData.isEqual(info);

    rANSCompressor compressor;

    compressor.encodeFile(PATH + inputFilename);
    compressor.decodeFile(PATH + "encoded.txt", PATH + "symbolInformations.txt");

    bool result = Comparer::compareFiles(PATH + inputFilename, PATH + "decoded.txt");
    std::cout << (result == true ? "File encoded and decoded successfully!\n" : "Something went wrong!\n") << std::endl;

    CompressionDetails fileEncodingDetails;
    fileEncodingDetails = compressor.getEncodingDetails();
    std::cout << "File: "<< inputFilename <<" encoding time: " << fileEncodingDetails.getOperationTime() << "[s]" << std::endl;
    std::cout << "File encoding speed: " << fileEncodingDetails.getOperationSpeed() << "[B/s]" << std::endl;
    std::cout << "File size after encoding: " << fileEncodingDetails.getObjectSize() << "[B]" << std::endl;
    std::cout << std::endl;

    CompressionDetails fileDecodingDetails;
    fileDecodingDetails = compressor.getDecodingDetails();
    std::cout << "File: " << "encoded.txt" << " decoding time: " << fileDecodingDetails.getOperationTime() << "[s]" << std::endl;
    std::cout << "File decoding speed: " << fileDecodingDetails.getOperationSpeed() << "[B/s]" << std::endl;
    std::cout << "File size after decoding: " << fileDecodingDetails.getObjectSize() << "[B]" << std::endl;
    std::cout << std::endl;

    double filenom = fileEncodingDetails.getObjectSize();
    double filede = fileDecodingDetails.getObjectSize();
    double fileCompressionRation = filenom / filede;
    std::cout << "File compression ratio: " << fileCompressionRation << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

#endif
    // --------------------------------------------------------------------------------------------
#if rANS_test_MESSAGE == 1

    std::cout << "-------------------- Encoding and decoding message --------------------\n\n";

    std::shared_ptr<std::string> messageBuffer = std::make_shared<std::string>();
    *messageBuffer = "Ala ma kota a kot ma Ale!";
    //*messageBuffer = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    //std::string message = "ala ma kota a kot ma ale!";
    std::string encodedMessage = "";
    std::string decodedMessage = "";

    SymbolInformation info(*messageBuffer);
    rANSCompressor messageCompressor;

    encodedMessage = messageCompressor.encode(*messageBuffer, info);
    decodedMessage = messageCompressor.decode(info, encodedMessage);

    if (decodedMessage == *messageBuffer) {
        std::cout << "Encoded and decoded succesfully!\n";
    }
    else {
        std::cout << "Something went wrong!\n";
    }
    
    CompressionDetails messageEncodingDetails;
    messageEncodingDetails = messageCompressor.getEncodingDetails();
    std::cout << "Message encoding time: " << messageEncodingDetails.getOperationTime() << "[s]" << std::endl;
    std::cout << "Message encoding speed: " << messageEncodingDetails.getOperationSpeed() << "[B/s]" << std::endl;
    std::cout << "Message size after encoding: " << messageEncodingDetails.getObjectSize() << "[B]" << std::endl;
    std::cout << std::endl;

    CompressionDetails messageDecodingDetails;
    messageDecodingDetails = messageCompressor.getDecodingDetails();
    std::cout << "Message decoding time: " << messageDecodingDetails.getOperationTime() << "[s]" << std::endl;
    std::cout << "Message decoding speed: " << messageDecodingDetails.getOperationSpeed() << "[B/s]" << std::endl;
    std::cout << "Message size after decoding: " << messageDecodingDetails.getObjectSize() << "[B]" << std::endl;
    std::cout << std::endl;

    double nom = messageDecodingDetails.getObjectSize();
    double de = messageEncodingDetails.getObjectSize();
    double messageCR = nom/de;
    std::cout << "Message compression ratio: " << messageCR << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;
#endif
    // --------------------------------------------------------------------------------------------
#if rANS_test_FILE_TO_CONSOLE == 1

    std::cout << "------------------------- From file -------------------------\n";
   
    std::shared_ptr<std::string> fromFileBuff = std::make_shared<std::string>();
    std::string path = PATH + "data.txt";
    std::string encodedFile = "";
    std::string decodedFile = "";

    SymbolInformation infoFromFile;
    fromFileBuff = infoFromFile.loadDataFromFile(path);

    rANSCompressor encoder;
    encodedFile = encoder.encode(*fromFileBuff, infoFromFile);
    decodedFile = encoder.decode(infoFromFile, encodedFile);

    if (decodedFile == *fromFileBuff) {
        std::cout << "Encoded and decoded succesfully!\n";
    }
    else {
        std::cout << "Something went wrong!\n";
    }

    CompressionDetails fileEncodingDetails;
    fileEncodingDetails = encoder.getEncodingDetails();
    std::cout << "File encoding time: " << fileEncodingDetails.getOperationTime() << "[s]" << std::endl;
    std::cout << "File encoding speed: " << fileEncodingDetails.getOperationSpeed() << "[B/s]" << std::endl;
    std::cout << "File size after encoding: " << fileEncodingDetails.getObjectSize() << "[B]" << std::endl;
    std::cout << std::endl;

    CompressionDetails fileDecodingDetails;
    fileDecodingDetails = encoder.getDecodingDetails();
    std::cout << "File decoding time: " << fileDecodingDetails.getOperationTime() << "[s]" << std::endl;
    std::cout << "File decoding speed: " << fileDecodingDetails.getOperationSpeed() << "[B/s]" << std::endl;
    std::cout << "File size after decoding: " << fileDecodingDetails.getObjectSize() << "[B]" << std::endl;
    std::cout << std::endl;

    double nomFile = fileDecodingDetails.getObjectSize();
    double deFile = fileEncodingDetails.getObjectSize();
    double fileCR = nomFile / deFile;
    std::cout << "File compression ratio: " << fileCR << std::endl;

#endif

    std::cin.get();
}
