#include "ZeroAndOne.h"
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

using namespace std;

// вспомогательные функции
static vector<uint8_t> fileToBits(const string& inputFile) {
    ifstream fin(inputFile, ios::binary);
    vector<uint8_t> bits;

    char byte;
    while (fin.read(&byte, 1)) {
        uint8_t u = static_cast<uint8_t>(byte);
        for (int i = 7; i >= 0; --i) {
            bits.push_back( (u >> i) & 1 );
        }
    }
    return bits;
}

static void bitsToFile(const vector<uint8_t>& bits, const string& outputFile) {
    ofstream fout(outputFile, ios::binary);

    uint8_t buffer = 0;
    int count = 0;

    for (uint8_t b : bits) {
        buffer = (buffer << 1) | (b & 1);
        count++;

        if (count == 8) {
            fout.put(static_cast<char>(buffer));
            buffer = 0;
            count = 0;
        }
    }

    if (count != 0) {
        buffer <<= (8 - count);
        fout.put(static_cast<char>(buffer));
    }
}

static vector<uint8_t> encodeBits(const vector<uint8_t>& a) {
    if (a.empty()) return {};

    vector<uint8_t> b;
    b.reserve(a.size());

    b.push_back(a[0]);  // b1 = a1

    for (size_t i = 1; i < a.size(); i++) {
        b.push_back(a[i] == a[i - 1] ? 1 : 0);
    }
    return b;
}

static vector<uint8_t> decodeBits(const vector<uint8_t>& b) {
    if (b.empty()) return {};

    vector<uint8_t> a;
    a.reserve(b.size());

    a.push_back(b[0]); // a1 = b1

    for (size_t i = 1; i < b.size(); i++) {
        if (b[i] == 1)
            a.push_back(a[i - 1]);
        else
            a.push_back(a[i - 1] ^ 1); // инверсия
    }
    return a;
}

// экспортируемые функции
extern "C" {

bool zeroandoneEnc(const string& inputFile, const string& outputFile) {
    vector<uint8_t> a = fileToBits(inputFile);
    vector<uint8_t> b = encodeBits(a);
    bitsToFile(b, outputFile);
    return true;
}

bool zeroandoneDec(const string& inputFile, const string& outputFile) {
    vector<uint8_t> b = fileToBits(inputFile);
    vector<uint8_t> a = decodeBits(b);
    bitsToFile(a, outputFile);
    return true;
}

}
