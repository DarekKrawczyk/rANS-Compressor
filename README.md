# Introduction
**rANS** (renormalization-based Asymmetric Numerical Systems) compression is a class of entropy coding algorithms that achieves high compression ratios by encoding data using adaptive probability distributions. The rANS algorithm, developed by **Jarek Duda**, is a variant of the Asymmetric Numeral Systems (ANS) family of entropy encoding techniques.It has gained significant attention in the field of lossless data compression due to its simplicity, efficiency, and competitive compression performance compared to other algorithms.

Techniques used in this project are based on implementation and approach of **Fabian “ryg” Giesen**'s methods shown in his ryg_rans repository which can be found under:
* [https://github.com/rygorous/ryg_rans](https://github.com/rygorous/ryg_rans)

### Sources used to create this project
* [Asymmetric numeral systems: entropy coding combining speed of Huffman coding with compression rate of arithmetic coding by **Jarek Duda**](https://arxiv.org/abs/1311.2540)
* [Asymmetric Numeral Systems by **Stanford University**](https://www.youtube.com/watch?v=6vNBLGKsssA&ab_channel=StanfordResearchTalks)
* [Understanding the new entropy coder family
by **Kedar Tatwawadi**](https://kedartatwawadi.github.io/post--ANS/)

This project aims to provide a implementation of rANS compression techniques, along with supporting utilities and tools to facilitate experimentation and evaluation.

# Content
* Experiment - compressing “Pan Tadeusz” book by Adam Mickiewicz.
* Example code - how to use rANS compressor.
* Classes overview - description of classes and its functionalities.

# Example Compression of "Pan Taduesz" Book
As part of an experiment, I ran the rANS compressor on the “Pan Tadeusz” book written by Adam Mickiewicz. The experiment was repeated 10 times, and the average results were as follows:

* Average Encoding Time: 0.414653 [s]

* Average Decoding Time: 0.600924 [s]

* Average Encoding Speed: 682,688 [B/s]

* Average Decoding Speed: 762,990 [B/s]

* Average Compression Ratio: 1.61929

# Example code
The example code provided showcases how to use the rANS Compression program to compress and decompress data. By running multiple experiments and calculating the average results, it provides insights into the performance and effectiveness of the compression algorithm.
```cpp
#include <iostream>
#include <list>
#include <string>
#include "SymbolInformation.hpp"
#include "rANSCompressor.hpp"
#include "Comparer.hpp"

#define EXAMPLE_DATA_PATH "D:\\Dev\\rANS Compressor\\rANS Compressor\\data\\"
#define PAN_TADEUSZ "PanTadeusz"

using namespace rANS;

int main()
{
    std::string PATH = EXAMPLE_DATA_PATH;
    std::string fileName = PAN_TADEUSZ;

    const size_t experiments = 10;

    double encodeSizeTab[experiments]{ 0,0,0,0,0,0,0,0,0,0 };
    double decodeSizeTab[experiments]{ 0,0,0,0,0,0,0,0,0,0 };
    double compressionRationTab[experiments]{ 0,0,0,0,0,0,0,0,0,0 };
    double encodingSpeed[experiments]{ 0,0,0,0,0,0,0,0,0,0 };
    double decodingSpeed[experiments]{ 0,0,0,0,0,0,0,0,0,0 };
    double encodingTime[experiments]{ 0,0,0,0,0,0,0,0,0,0 };
    double decodingTime[experiments]{ 0,0,0,0,0,0,0,0,0,0 };

    SymbolInformation symInfo;
    symInfo.loadDataFromFile(PATH + fileName);

    uint32_t encodingCode = 0;
    uint32_t decodingCode = 0;

    for (size_t i = 0; i < experiments; i++) {

        rANSCompressor rAnsCompressor;
        encodingCode = rAnsCompressor.encodeFile(PATH + fileName);
        decodingCode = rAnsCompressor.decodeFile(PATH + "encoded", PATH + "symbolInformations.txt");

        std::cout << "--------------- Operation: " << i << "---------------------\n";
        std::cout << "Encoding code: " << encodingCode << std::endl;
        std::cout << "Decoding code: " << decodingCode << std::endl;
        std::cout << std::endl;

        bool result = Comparer::compareFiles(PATH + fileName, PATH + "decoded");
        std::cout << (result == true ? "File encoded and decoded successfully!\n" : "Something went wrong!\n");

        CompressionDetails encDetails = rAnsCompressor.getEncodingDetails();
        double encTime = encDetails.getOperationTime();
        double encSpeed = encDetails.getOperationSpeed();
        size_t encSize = encDetails.getObjectSize();
        std::cout << "File: " << fileName << " encoding time: " << encTime << "[s]" << std::endl;
        std::cout << "File encoding speed: " << encSpeed << "[B/s]" << std::endl;
        std::cout << "File size after encoding: " << encSize << "[B]" << std::endl;
        std::cout << std::endl;

        CompressionDetails decDetails = rAnsCompressor.getDecodingDetails();
        double decTime = decDetails.getOperationTime();
        double decSpeed = decDetails.getOperationSpeed();
        size_t decSize = decDetails.getObjectSize();
        std::cout << "File: " << "encoded.txt" << " decoding time: " << decTime << "[s]" << std::endl;
        std::cout << "File decoding speed: " << decSpeed << "[B/s]" << std::endl;
        std::cout << "File size after decoding: " << decSize << "[B]" << std::endl;
        std::cout << std::endl;

        double fileCompressionRation = (double)decSize / (double)encSize;
        std::cout << "File compression ratio: " << fileCompressionRation << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;

        //Save results
        encodeSizeTab[i] = encSize;
        decodeSizeTab[i] = decSize;
        compressionRationTab[i] = fileCompressionRation;
        encodingSpeed[i] = encSpeed;
        decodingSpeed[i] = decSpeed;
        encodingTime[i] = encTime;
        decodingTime[i] = decTime;
    }

    //Results
    std::cout << "--------------- Results ---------------------\n";
    std::cout << "Avg encoding time: " << CompressionDetails::CalculateAvg(encodingTime, experiments) << "[s]" << std::endl;
    std::cout << "Avg decoding time: " << CompressionDetails::CalculateAvg(decodingTime, experiments) << "[s]" << std::endl;
    std::cout << "Avg encoding speed: " << CompressionDetails::CalculateAvg(encodingSpeed, experiments) << "[B/s]" << std::endl;
    std::cout << "Avg decoding speed: " << CompressionDetails::CalculateAvg(decodingSpeed, experiments) << "[B/s]" << std::endl;
    std::cout << "Avg compression ratio: " << CompressionDetails::CalculateAvg(compressionRationTab, experiments) << std::endl;
}
```

# Classes overview
The rANS Compression project consists of several classes that encapsulate the functionality of the compression and decompression operations. These classes include the rANSCompressor class, which handles the encoding and decoding processes, and the SymbolInformation class, which manages information about the data to be compressed. Additionally, the CompressionDetails class stores and calculates various details related to the compression process, such as encoding time, decoding time, compression ratio, and encoding/decoding speeds.

# rANSCompressor
The **rANSCompressor** class is a implementation of the rANS compression algorithm in **C++**. It provides methods for encoding and decoding data.

## Constructors
>**rANSCompressor()**: Constructs an empty rANSCompressor object.

>**rANSCompressor(const SymbolInformation& info)**: Constructs an rANSCompressor object with the provided symbol information.

>**rANSCompressor(const SymbolInformation\* info)**: Constructs an rANSCompressor object with the symbol information obtained from the provided pointer.

## Destructor
>**~rANSCompressor()**: Destroys the rANSCompressor object and frees any allocated resources.

## Encoding and Decoding Methods
> **uint32_t encodeFile(std::string encodePath)**: Encodes the contents of the file specified by encodePath using rANS compression. Returns the size of the encoded file in bytes.

>**uint32_t decodeFile(std::string decodePath, std::string symbolInfoPath)**: Decodes the contents of the file specified by decodePath using rANS decompression and the symbol information provided in the file specified by symbolInfoPath. Returns the size of the decoded file in bytes.

>**std::shared_ptr<std::list<uint8_t>> encode(const std::shared_ptr<std::list<uint8_t>>& dataBuffer, const SymbolInformation& info)**: Encodes the data contained in the dataBuffer using rANS compression with the provided symbol information (info). Returns a shared pointer to a list of encoded data bytes.

>**std::shared_ptr<std::list<uint8_t>> decode(const SymbolInformation& info, const std::shared_ptr<std::list<uint8_t>>& encodedData)**: Decodes the data contained in the encodedData using rANS decompression with the provided symbol information (info). Returns a shared pointer to a list of decoded data bytes.

## Utility Methods
>**void printData()**: Prints the data stored in the rANSCompressor object.

>**CompressionDetails getEncodingDetails() const**: Retrieves the details of the encoding process, including compression ratio, time taken, etc. Returns a CompressionDetails object containing encoding details.

>**CompressionDetails getDecodingDetails() const**: Retrieves the details of the decoding process, including decompression ratio, time taken, etc. Returns a CompressionDetails object containing decoding details.

# SymbolInformation
The **SymbolInformation** class is responsible for holding and managing information about the data to be compressed. It calculates various metrics and provides methods for loading data, saving data to files, and performing operations on the symbol information. Use the SymbolInformation class to load, manage, and manipulate the symbol information data used for compression.

## Constructors
>**SymbolInformation()**: Constructs an empty SymbolInformation object.

>**SymbolInformation(const std::shared_ptr<std::list<uint8_t>>& dataBuffer)**: Constructs a SymbolInformation object and initializes it with the provided data buffer.

>**SymbolInformation(const SymbolInformation& other)**: Constructs a SymbolInformation object by copying the contents of another SymbolInformation object.

>**SymbolInformation(const SymbolInformation\* other)**: Constructs a SymbolInformation object by copying the contents of the SymbolInformation object pointed to by other.

## Destructor
>**~SymbolInformation()**: Destroys the SymbolInformation object and frees any allocated resources.
Loading and Saving Data

>**static std::shared_ptr<std::list<uint8_t>> LoadBuffer(std::string path)**: Loads the contents of the file specified by path into a shared pointer to a list of data bytes. Returns the loaded data buffer.

>**std::shared_ptr<std::list<uint8_t>> loadDataFromFile(std::string path)**: Loads the contents of the file specified by path into the current SymbolInformation object. Returns a shared pointer to the loaded data buffer.

>**static std::shared_ptr<std::list<uint8_t>> ToBuffer(const std::string& data)**: Converts the provided string data into a shared pointer to a list of data bytes. Returns the converted data buffer.

>**void toFile()**: Saves the symbol information data to a file.

>**bool loadSymbolInfoFromFile(std::string path)**: Loads the symbol information from the file specified by path into the current SymbolInformation object. Returns true if successful, and false otherwise.

## Utility Methods
>**void printData()**: Prints the symbol information data.

>**void calculateSymbolsInformation()**: Calculates various metrics and information about the symbols in the data buffer.

>**void clearData()**: Clears the symbol information data.

>**bool isEqual(const SymbolInformation& other)**: Compares the current SymbolInformation object with another SymbolInformation object (other). Returns true if they are equal, and false otherwise.

# CompressionDetails
The **CompressionDetails** class is responsible for storing and calculating information about the compression operation, such as the time taken and the size of the compressed object. It provides methods for setting and retrieving these details. Use the CompressionDetails class to store and calculate information about the compression operation, such as time, speed, and size. 


## Constructor
>**CompressionDetails()**: Constructs an empty CompressionDetails object.

## Accessors

>**void setOperationTime(std::chrono::duration<double> time)**: Sets the time taken for the compression operation specified by the time duration.

>**void setObjectSize(size_t size)**: Sets the size of the compressed object to the specified size.

## Calculation
>**void calculate()**: Calculates additional details based on the set operation time and object size.
Retrieving Details
>**double getOperationTime() const**: Retrieves the time taken for the compression operation in seconds.

>**double getOperationSpeed() const**: Retrieves the speed of the compression operation in units per second.

>**size_t getObjectSize() const**: Retrieves the size of the compressed object in bytes.