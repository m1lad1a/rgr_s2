#include "FixedShift.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

// вспомогательные функции
vector<int> getInversePermutation(const vector<int>& perm) {

    vector<int> inv(perm.size());

    for (size_t i = 0; i < perm.size(); ++i) inv[perm[i]-1] = i + 1;
    return inv;
}

void encryptBlock(vector<char>& block, const vector<int>& perm) {

    size_t k = perm.size();
    size_t original_size = block.size();
    size_t padding = (k - original_size % k) % k;

    block.resize(original_size + padding, ' ');
    vector<char> temp(block.size());

    for (size_t i = 0; i < block.size(); i += k)
        for (size_t j = 0; j < k; ++j) temp[i + j] = block[i + perm[j]-1];

    block = move(temp);
}

void decryptBlock(vector<char>& block, const vector<int>& perm) {

    size_t k = perm.size();
    vector<int> inv = getInversePermutation(perm);
    vector<char> temp(block.size());

    for (size_t i = 0; i < block.size(); i += k)
        for (size_t j = 0; j < k; ++j)
            temp[i + j] = block[i + inv[j] - 1];

    block = move(temp);
}

// экспортируемые функции
extern "C" {

bool isValidPermutation(const vector<int>& perm) {
    if (perm.empty()) return false;

    vector<bool> seen(perm.size(), false);
    
    for (int num : perm) {
        if (num < 1 || num > static_cast<int>(perm.size()) || seen[num - 1]) return false;
        seen[num - 1] = true;
    }
    return true;
}

void fixedshiftEnc(const string& inputFile, const string& outputFile, const vector<int>& perm) {
    if (!isValidPermutation(perm)) return;

    ifstream fin(inputFile, ios::binary);
    if (!fin.is_open()) return;

    ofstream fout(outputFile, ios::binary);
    if (!fout.is_open()) return;

    constexpr size_t BLOCK_SIZE = 1024 * 1024;
    vector<char> buffer(BLOCK_SIZE);

    while (fin.read(buffer.data(), BLOCK_SIZE) || fin.gcount() > 0) {
        buffer.resize(static_cast<size_t>(fin.gcount()));
        encryptBlock(buffer, perm);
        fout.write(buffer.data(), buffer.size());
    }
}

void fixedshiftDec(const string& inputFile, const string& outputFile, const vector<int>& perm) {
    if (!isValidPermutation(perm)) return;

    ifstream fin(inputFile, ios::binary);
    if (!fin.is_open()) return;

    ofstream fout(outputFile, ios::binary);
    if (!fout.is_open()) return;

    constexpr size_t BLOCK_SIZE = 1024 * 1024;
    vector<char> buffer(BLOCK_SIZE);

    while (fin.read(buffer.data(), BLOCK_SIZE) || fin.gcount() > 0) {
        size_t readBytes = static_cast<size_t>(fin.gcount());
        buffer.resize(readBytes);

        decryptBlock(buffer, perm);

        if (fin.peek() == EOF) {
            while (!buffer.empty() && buffer.back() == ' ')
                buffer.pop_back();
        }

        fout.write(buffer.data(), buffer.size());
    }
}

}
