#ifndef CRYPTO_H
#define CRYPTO_H

#include "FixedShift.h"
#include "TableShift.h"
#include "ZeroAndOne.h"
#include <vector>
#include <string>

class Crypto {
public:
    enum Algorithm { ZEROANDONE = 1, TABLESHIFT = 2, FIXEDSHIFT = 3 };
    // меню
    void showMenu();  
    std::string getFilename(const std::string& prompt);
    std::string checkFilename(const std::string& prompt);  
    int chooseAlgorithm();  
    void clearScreen();  

    bool fileExists(const std::string& filename);
    void inputTextToFile();
    void encryptFile(); 
    void decryptFile(); 
    void displayFile(); 

    // алгоритмы шифрования
    bool isValidPermutation(const std::vector<int>& perm);
    void fixedshiftEnc(const std::string& inputFile, const std::string& outputFile, const std::vector<int>& perm);
    void fixedshiftDec(const std::string& inputFile, const std::string& outputFile, const std::vector<int>& perm);

    void tableshiftEnc(const std::string& inputFile, const std::string& outputFile,
        int rows, int cols, const std::string& keyFile);
    void tableshiftDec(const std::string& inputFile, const std::string& outputFile,
        const std::string& keyFile);

    bool zeroandoneEnc(const std::string& inputFile, const std::string& outputFile);
    bool zeroandoneDec(const std::string& inputFile, const std::string& outputFile);

    // работа с файлами
    std::vector<unsigned char> readBinaryFile(const std::string& filename);
    void writeBinaryFile(const std::string& filename, const std::vector<unsigned char>& data);
};

#endif
