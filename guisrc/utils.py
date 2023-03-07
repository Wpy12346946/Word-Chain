from ctypes import *
import re


def split2bytes(s):
    wordList = re.compile('[a-zA-z]+').findall(s)
    length = len(wordList)
    words = (c_char_p * length)()
    for p in range(length):
        b_str = bytes(wordList[p].encode('utf-8')).lower()
        words[p] = b_str

    return words, c_int(length)


def bytes2str(words, length):
    res = ""
    for i in range(length):
        res += words[i].decode()
        res += "\n"
    return res
