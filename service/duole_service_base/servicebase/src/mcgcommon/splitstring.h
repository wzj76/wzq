#ifndef __SPLIT_STRING_H__
#define __SPLIT_STRING_H__

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class CSplitString
{
public:
	CSplitString(const string& sSrc, const string sSeparateCharacter);
	virtual ~CSplitString();

public:
	size_t GetSize();
	string GetString(const size_t iIndex);

private:
	vector <string> m_vectorStrArray;
};

#endif // __SPLIT_STRING_H__



