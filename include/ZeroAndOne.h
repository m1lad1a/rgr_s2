#ifndef ZEROANDONE_H
#define ZEROANDONE_H

#include <string>

#ifdef __cplusplus
extern "C" {
#endif

bool zeroandoneEnc(const std::string& inputFile, const std::string& outputFile);
bool zeroandoneDec(const std::string& inputFile, const std::string& outputFile);

#ifdef __cplusplus
}
#endif

#endif