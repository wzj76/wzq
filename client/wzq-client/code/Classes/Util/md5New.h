#pragma once
#ifndef MD5New_H
#define MD5New_H

#include <string>
#include <fstream>

/* Type define */
typedef unsigned char byte;
typedef unsigned int uint32;

using std::string;
using std::ifstream;

/* MD5New declaration. */
class MD5New {
public:
    MD5New();
    MD5New(const void* input, size_t length);
    MD5New(const string& str);
    MD5New(ifstream& in);
    void update(const void* input, size_t length);
    void update(const string& str);
    void update(ifstream& in);
    const byte* digest();
    string toString();
    void reset();

private:
    void update(const byte* input, size_t length);
    void final();
    void transform(const byte block[64]);
    void encode(const uint32* input, byte* output, size_t length);
    void decode(const byte* input, uint32* output, size_t length);
    string bytesToHexString(const byte* input, size_t length);

    /* class uncopyable */
    MD5New(const MD5New&);
    MD5New& operator=(const MD5New&);

private:
    uint32 _state[4]; /* state (ABCD) */
    uint32 _count[2]; /* number of bits, modulo 2^64 (low-order word first) */
    byte _buffer[64]; /* input buffer */
    byte _digest[16]; /* message digest */
    bool _finished;   /* calculate finished ? */

    static const byte PADDING[64]; /* padding for calculate */
    static const char HEX[16];
    enum { BUFFER_SIZE = 1024 };
};

#endif /*MD5New_H*/
