#!/usr/bin/python
# -*- coding: UTF-8 -*-
import sys, getopt
import subprocess
import os
import json
import inspect
import shutil
import struct 

# xxtea 加密、解密方法；
_DELTA = 0x9E3779B9
def _long2str(v, w):  
    n = (len(v) - 1) << 2  
    if w:  
        m = v[-1]  
        if (m < n - 3) or (m > n): return ''  
        n = m  
    s = struct.pack('<%iL' % len(v), *v)  
    return s[0:n] if w else s  
  
def _str2long(s, w):  
    n = len(s)  
    m = (4 - (n & 3) & 3) + n  
    s = s.ljust(m, "\0")  
    v = list(struct.unpack('<%iL' % (m >> 2), s))  
    if w: v.append(n)  
    return v  
  
def encrypt(str, key):  
    if str == '': return str  
    v = _str2long(str, True)  
    k = _str2long(key.ljust(16, "\0"), False)  
    n = len(v) - 1  
    z = v[n]  
    y = v[0]  
    sum = 0  
    q = 6 + 52 // (n + 1)  
    while q > 0:  
        sum = (sum + _DELTA) & 0xffffffff  
        e = sum >> 2 & 3  
        for p in xrange(n):  
            y = v[p + 1]  
            v[p] = (v[p] + ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z))) & 0xffffffff  
            z = v[p]  
        y = v[0]  
        v[n] = (v[n] + ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[n & 3 ^ e] ^ z))) & 0xffffffff  
        z = v[n]  
        q -= 1  
    return _long2str(v, False)  
  
def decrypt(str, key):  
    if str == '': return str  
    v = _str2long(str, False)  
    k = _str2long(key.ljust(16, "\0"), False)  
    n = len(v) - 1  
    z = v[n]  
    y = v[0]  
    q = 6 + 52 // (n + 1)  
    sum = (q * _DELTA) & 0xffffffff  
    while (sum != 0):  
        e = sum >> 2 & 3  
        for p in xrange(n, 0, -1):  
            z = v[p - 1]  
            v[p] = (v[p] - ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[p & 3 ^ e] ^ z))) & 0xffffffff  
            y = v[p]  
        z = v[n]  
        v[0] = (v[0] - ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (k[0 & 3 ^ e] ^ z))) & 0xffffffff  
        y = v[0]  
        sum = (sum - _DELTA) & 0xffffffff  
    return _long2str(v, True)  

# xxtea 加密
def xxteaEncrypt(key, sign, src, dest):
    shutil.copy(src, dest)
    bytesFile = open(dest, "rb+")
    encryBytes = encrypt(bytesFile.read(), key)
    encryBytes = sign + encryBytes
    bytesFile.seek(0)
    bytesFile.truncate()
    bytesFile.write(encryBytes)
    bytesFile.close()

# xxtea 解密
def xxteaDecrypt(key, sign, src, dest):
    shutil.copy(src, dest)
    bytesFile = open(dest, "rb+")
    # 从加密文件读取并校验签名
    _tmpSign = bytesFile.read(len(sign))
    if _tmpSign != sign:
        print '签名验证失败:', _tmpSign, '!=' , xxtesigna_sign
        exit()

    bytesFile.seek(len(sign))
    decryptBytes = decrypt(bytesFile.read(), key)    
    bytesFile.seek(0)
    bytesFile.truncate()
    bytesFile.write(decryptBytes)
    bytesFile.close()

def usage():
    print 'usage: xxtea_crypto.py -m <mode> -k <key> -s <sign> -i <src> -o <dest>'
def main(argv):
    # 检查参数个数
    if len(sys.argv) != 11 :
        usage()
        sys.exit()
    try:
        opts, args = getopt.getopt(argv, "hm:k:s:i:o:", ["mmode=","kkey=","ssign=","isrc=","odest="])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    
    # 参数解析
    for opt, arg in opts:
        if opt == '-h':
            usage()
            sys.exit()
        elif opt in ("-m", "--mmode"):
            mode = arg
        elif opt in ("-k", "--kkey"):
            xxtea_key = arg
        elif opt in ("-s", "--ssign"):
            xxtea_sign = arg
        elif opt in ("-i", "--isrc"):
            srcFile = arg
        elif opt in ("-o", "--odest"):
            dstFile = arg
        else :
            usage()
            sys.exit()

    # print info:
    print mode, ':', srcFile, '->', dstFile
    print 'key:', xxtea_key, ' sign:', xxtea_sign
    if mode != 'encrypt':
        # 解密
        xxteaDecrypt(xxtea_key, xxtea_sign, srcFile, dstFile)
    else :
        # 加密
        xxteaEncrypt(xxtea_key, xxtea_sign, srcFile, dstFile)

if __name__ == "__main__":
    main(sys.argv[1:])
