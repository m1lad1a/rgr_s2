#ifndef TABLESHIFT_H
#define TABLESHIFT_H

#include <string>

#ifdef __cplusplus
extern "C" {
#endif

void tableshiftEnc(const std::string& inputFile, const std::string& outputFile,
                 int rows, int cols, const std::string& keyFile);

void tableshiftDec(const std::string& inputFile, const std::string& outputFile,
                 const std::string& keyFile);

#ifdef __cplusplus
}
#endif

#endif