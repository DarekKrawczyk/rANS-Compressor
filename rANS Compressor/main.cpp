#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <math.h>
#include <filesystem>
#include "SymbolInformation.hpp"
#include "rANSCompressor.hpp"
#include "Comparer.hpp"

#define rANS_test_FILE_TO_CONSOLE 0
#define rANS_test_FILE 0
#define rANS_test_MESSAGE 1

#define EXAMPLE_DATA_PATH "D:\\Dev\\rANS Compressor\\rANS Compressor\\data\\"

#define LONG_LOREM_IPSUM "longLoremIpsum"
#define BOOK "book"
#define BOOK_PART "bookPart"
#define PAN_TADEUSZ "PanTadeusz"

using namespace rANS;

int main()
{
    std::string PATH = EXAMPLE_DATA_PATH;

#if rANS_test_FILE == 1

    std::cout << "-------------------- Encoding and decoding file --------------------\n\n";

    std::shared_ptr<std::list<uint8_t>> buffer = std::make_shared<std::list<uint8_t>>();
    std::string inputFilename = PAN_TADEUSZ;

    SymbolInformation testingData;
    buffer = testingData.loadDataFromFile(PATH + inputFilename);
    testingData.toFile();

    SymbolInformation info;
    info.loadSymbolInfoFromFile(PATH + "symbolInformations.txt");

    bool same = testingData.isEqual(info);

    rANSCompressor compressor;

    compressor.encodeFile(PATH + inputFilename);
    compressor.decodeFile(PATH + "encoded", PATH + "symbolInformations.txt");

    bool result = Comparer::compareFiles(PATH + inputFilename, PATH + "decoded");
    std::cout << (result == true ? "File encoded and decoded successfully!\n" : "Something went wrong!\n");

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

    double fileEncodedSize = fileEncodingDetails.getObjectSize();
    double fileDecodedSize = fileDecodingDetails.getObjectSize();
    double fileCompressionRation = fileDecodedSize / fileEncodedSize;
    std::cout << "File compression ratio: " << fileCompressionRation << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

#endif
    // --------------------------------------------------------------------------------------------
#if rANS_test_MESSAGE == 1

    std::cout << "-------------------- Encoding and decoding message --------------------\n\n";

    std::shared_ptr<std::list<uint8_t>> messageBuffer = std::make_shared<std::list<uint8_t>>();
    messageBuffer = SymbolInformation::ToBuffer("R$9+d>jG#X:krw=J7Qh8@B6?4]Oc]0Fg*U1LP|sD_#MIzCafHvV2[AeE<x}l'oY{pK;XhZ,ui3N5Tn6MSty{Bv/Wq]@gqOA(]Jm$45^7<3L`Y%b>S)py4^G*&aB9vJn%Q1{UzXDsw<]qY6cLf)N2O]KsMx^%iVgC(0@Hm8W3Z!+kt#oR}d?r/E'1u;pbT75eFI.AhY>6<SX]LD<P)vS+jzy:f?{xqKoC!4Rn3Gs<2}dX%tZ*h@8q[L)nC(3yFzN]U:p-vE?6Ku9l4Q/~R+aY1.5G_7WMsOc;VI\"BXm<0fkD> | bH = SgJr'TPwiA{#^,j]$o!>Xz\([u^v;}KxNQF=L6Ytp2m+10CV.:hIsH3D_]7fAyZn$br&OGa}S\"9q%!Wx~;]4JASDSDSDSDSDSDSDSDSDSDSDSDSDSDSDSDSD4JASDSDSDSDSDSDSDSDSDSDSDSDSDSDSDSDSD4JASDSDSDSDSDSDSDSDSDSDSDSDSDSDSDSDSD4JASDSDSDSDSDSDSDSDSDSDSDSDSDSDSDSDSD4JASDSDSDSDSDSDSDSDSDSDSDSDSDSDSDSDSD4J");
    //messageBuffer = SymbolInformation::ToBuffer("On the other hand, we denounce with righteous indignation and dislike men who are so beguiled and demoralized by the charms of pleasure of the moment, so blinded by desire, that they cannot foresee the pain and trouble that are bound to ensue; and equal blame belongs to those who fail in their duty through weakness of will, which is the same as saying through shrinking from toil and pain. These cases are perfectly simple and easy to distinguish. In a free hour, when our power of choice is untrammelled and when nothing prevents our being able to do what we like best, every pleasure is to be welcomed and every pain avoided. But in certain circumstances and owing to the claims of duty or the obligations of business it will frequently occur that pleasures have to be repudiated and annoyances accepted. The wise man therefore always holds in these matters to this principle of selection: he rejects pleasures to secure other greater pleasures, or else he endures pains to avoid worse pains.O");
    //messageBuffer = SymbolInformation::ToBuffer("Ala ma kota a kot ma Ale!");
    //messageBuffer = SymbolInformation::ToBuffer("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

    std::shared_ptr<std::list<uint8_t>> encodedMessage = std::make_shared<std::list<uint8_t>>();
    std::shared_ptr<std::list<uint8_t>> decodedMessage = std::make_shared<std::list<uint8_t>>();

    SymbolInformation info(messageBuffer);
    rANSCompressor messageCompressor;

    encodedMessage = messageCompressor.encode(messageBuffer, info);
    decodedMessage = messageCompressor.decode(info, encodedMessage);

    if (*decodedMessage == *messageBuffer) {
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

    std::cout << "-------------------- Encoding and decoding from file to console --------------------\n\n";
   
    std::shared_ptr<std::list<uint8_t>> fromFileBuff = std::make_shared<std::list<uint8_t>>();
    std::string fileName = PAN_TADEUSZ;

    std::string path = PATH + fileName;
    std::shared_ptr<std::list<uint8_t>> encodedFile = std::make_shared<std::list<uint8_t>>();
    std::shared_ptr<std::list<uint8_t>> decodedFile = std::make_shared<std::list<uint8_t>>();

    SymbolInformation infoFromFile;
    fromFileBuff = infoFromFile.loadDataFromFile(path);
    infoFromFile.toFile();

    rANSCompressor encoder;
    encodedFile = encoder.encode(fromFileBuff, infoFromFile);
    decodedFile = encoder.decode(infoFromFile, encodedFile);

    if (*decodedFile == *fromFileBuff) {
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
}
