#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <string>
#include <vector>

//string memory management
typedef std::vector<char*> DynamicStringList;
char * mkstr(const char * value);
void clearDynamicStrings();

std::string getSequencedFile(const char * iPrefix, int iValue, const char * iPostfix, int iValueLength);

#endif //TESTUTILS_H
