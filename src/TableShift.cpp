#include "TableShift.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

// вспомогательные функции
static string encryptBlock(const string& block, int rows, int cols) {
    string filled = block;
    size_t size = rows * cols;
    if (filled.size() < size) filled.append(size - filled.size(), ' ');

    vector<vector<char>> table(rows, vector<char>(cols, ' '));
    size_t index = 0;
    for (int c = 0; c < cols; ++c)
        for (int r = 0; r < rows; ++r)
            table[r][c] = filled[index++];

    string result;
    result.reserve(size);
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            result.push_back(table[r][c]);

    return result;
}

static string decryptBlock(const string& block, int rows, int cols) {
    string filled = block;
    size_t size = rows * cols;
    if (filled.size() < size) filled.append(size - filled.size(), ' ');

    vector<vector<char>> table(rows, vector<char>(cols, ' '));
    size_t index = 0;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            table[r][c] = filled[index++];

    string result;
    result.reserve(size);
    for (int c = 0; c < cols; ++c)
        for (int r = 0; r < rows; ++r)
            result.push_back(table[r][c]);

    return result;
}

// экспортируемые функции
extern "C" {
    void tableshiftEnc(const string& inputFile, const string& outputFile,
                            int rows, int cols, const string& keyFile) {
        if (rows <= 0 || cols <= 0) return;

        ifstream in(inputFile, ios::binary);
        if (!in.is_open()) return;

        ofstream out(outputFile, ios::binary);
        if (!out.is_open()) return;

        ofstream keyOut(keyFile);
        if (!keyOut.is_open()) return;
        keyOut << rows << " " << cols;
        keyOut.close();

        const size_t blockSize = rows * cols;
        vector<char> buffer(blockSize);

        while (in.read(buffer.data(), blockSize) || in.gcount() > 0) {
            string block(buffer.data(), in.gcount());
            string encrypted = encryptBlock(block, rows, cols);
            out.write(encrypted.data(), encrypted.size());
        }
    }

    void tableshiftDec(const string& inputFile, const string& outputFile,
                            const string& keyFile) {
        ifstream keyIn(keyFile);
        if (!keyIn.is_open()) return;

        int rows, cols;
        keyIn >> rows >> cols;
        keyIn.close();

        ifstream in(inputFile, ios::binary);
        if (!in.is_open()) return;

        ofstream out(outputFile, ios::binary);
        if (!out.is_open()) return;

        const size_t blockSize = rows * cols;
        vector<char> buffer(blockSize);

        while (in.read(buffer.data(), blockSize) || in.gcount() > 0) {
            string block(buffer.data(), in.gcount());
            string decrypted = decryptBlock(block, rows, cols);
            out.write(decrypted.data(), decrypted.size());
        }
    }
}
