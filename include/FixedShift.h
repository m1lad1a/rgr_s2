#ifndef FIXEDSHIFT_H
#define FIXEDSHIFT_H

#include <string>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

bool isValidPermutation(const std::vector<int>& perm);
void fixedshiftEnc(const std::string& inputFile, const std::string& outputFile, const std::vector<int>& perm);
void fixedshiftDec(const std::string& inputFile, const std::string& outputFile, const std::vector<int>& perm);

#ifdef __cplusplus
}
#endif

#endif