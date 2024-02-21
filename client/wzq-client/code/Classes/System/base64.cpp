/*
 *
 *Base64?
 *
 *
 *
*/

#include "base64.h"
#include <vector>
static const char *g_pCodes =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

static const unsigned char g_pMap[256] =
{
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	 52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	  7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	 19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	 37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	 49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255
};

const char base64_url_alphabet[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'
};

CBase64::CBase64()
{
}

CBase64::~CBase64()
{
}

bool CBase64::Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen)
{
	unsigned long i, len2, leven;
	unsigned char *p;

	if(pOut == nullptr || *uOutLen == 0)
		return false;

	//ASSERT((pIn != nullptr) && (uInLen != 0) && (pOut != nullptr) && (uOutLen != nullptr));

	len2 = ((uInLen + 2) / 3) << 2;
	if((*uOutLen) < (len2 + 1)) return false;

	p = pOut;
	leven = 3 * (uInLen / 3);
	for(i = 0; i < leven; i += 3)
	{
		*p++ = g_pCodes[pIn[0] >> 2];
		*p++ = g_pCodes[((pIn[0] & 3) << 4) + (pIn[1] >> 4)];
		*p++ = g_pCodes[((pIn[1] & 0xf) << 2) + (pIn[2] >> 6)];
		*p++ = g_pCodes[pIn[2] & 0x3f];
		pIn += 3;
	}

	if (i < uInLen)
	{
		unsigned char a = pIn[0];
		unsigned char b = ((i + 1) < uInLen) ? pIn[1] : 0;
		unsigned char c = 0;

		*p++ = g_pCodes[a >> 2];
		*p++ = g_pCodes[((a & 3) << 4) + (b >> 4)];
		*p++ = ((i + 1) < uInLen) ? g_pCodes[((b & 0xf) << 2) + (c >> 6)] : '=';
		*p++ = '=';
	}

	*p = 0; // Append NULL byte
	*uOutLen = p - pOut;
	return true;
}

bool CBase64::Encode(const unsigned char *pIn, unsigned long uInLen, string& strOut) 
{
	unsigned long i, len2, leven;
	strOut = "";

	//ASSERT((pIn != nullptr) && (uInLen != 0) && (pOut != nullptr) && (uOutLen != nullptr));

	len2 = ((uInLen + 2) / 3) << 2;
	//if((*uOutLen) < (len2 + 1)) return false;

	//p = pOut;
	leven = 3 * (uInLen / 3);
	for(i = 0; i < leven; i += 3)
	{
		strOut += g_pCodes[pIn[0] >> 2];
		strOut += g_pCodes[((pIn[0] & 3) << 4) + (pIn[1] >> 4)];
		strOut += g_pCodes[((pIn[1] & 0xf) << 2) + (pIn[2] >> 6)];
		strOut += g_pCodes[pIn[2] & 0x3f];
		pIn += 3;
	}

	if (i < uInLen)
	{
		unsigned char a = pIn[0];
		unsigned char b = ((i + 1) < uInLen) ? pIn[1] : 0;
		unsigned char c = 0;

		strOut += g_pCodes[a >> 2];
		strOut += g_pCodes[((a & 3) << 4) + (b >> 4)];
		strOut += ((i + 1) < uInLen) ? g_pCodes[((b & 0xf) << 2) + (c >> 6)] : '=';
		strOut += '=';
	}

	//*p = 0; // Append NULL byte
	//*uOutLen = p - pOut;
	return true;
}

bool CBase64::Decode(const string& strIn, unsigned char *pOut, unsigned long *uOutLen) 
{
	unsigned long t, x, y, z;
	unsigned char c;
	unsigned long g = 3;

	//ASSERT((pIn != nullptr) && (uInLen != 0) && (pOut != nullptr) && (uOutLen != nullptr));

	for(x = y = z = t = 0; x < strIn.length(); x++)
	{
		c = g_pMap[strIn[x]];
		if(c == 255) continue;
		if(c == 254) { c = 0; g--; }

		t = (t << 6) | c;

		if(++y == 4)
		{
			if((z + g) > *uOutLen) { return false; } // Buffer overflow
			pOut[z++] = (unsigned char)((t>>16)&255);
			if(g > 1) pOut[z++] = (unsigned char)((t>>8)&255);
			if(g > 2) pOut[z++] = (unsigned char)(t&255);
			y = t = 0;
		}
	}

	*uOutLen = z;
	return true;
}

std::string CBase64::UrlEncode(const std::string & sData)
{
    std::string out;
    int val =0, valb=-6;
    size_t len = sData.length();
    unsigned int i = 0;
    for (i = 0; i < len; i++) {
        unsigned char c = sData[i];
        val = (val<<8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_url_alphabet[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) {
        out.push_back(base64_url_alphabet[((val<<8)>>(valb+8))&0x3F]);
    }
    return out;
}

std::string CBase64::UrlDecode(const std::string & sData)
{
    std::string out;
    std::vector<int> T(256, -1);
    unsigned int i;
    for (i =0; i < 64; i++) T[base64_url_alphabet[i]] = i;
    
    int val = 0, valb = -8;
    for (i = 0; i < sData.length(); i++) {
        unsigned char c = sData[i];
        if (T[c] == -1) break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}

