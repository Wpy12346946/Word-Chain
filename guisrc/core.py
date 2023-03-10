from ctypes import *
import time

import utils

# dll = WinDLL(r".\\core.dll")
dll = WinDLL("E:\\coding\\C++\\VisualStudio\\Word-Chain\\src\\out\\build\\x64-Debug\\core.dll")
WORD_CYCLE_EXCEPTION = 0x80000001
TOO_LONG_EXCEPTION = 0x80000002


def gen_chains_all(words: str):
    words, length = utils.split2bytes(words)
    result = (c_char_p * 1000)()

    startTime = time.time()
    chainLen = dll.gen_chains_all(words, length, result)
    endTime = time.time()

    if chainLen < 0:
        raise WordException(chainLen)

    res = utils.bytes2str(result, chainLen)
    times = endTime - startTime
    return res, chainLen, times


def gen_chain_word(words: str, head: int, tail: int, reject: int, enable_loop: bool):
    words, length = utils.split2bytes(words)
    result = (c_char_p * 1000)()
    head = c_char(head)
    tail = c_char(tail)
    reject = c_char(reject)
    enable_loop = c_bool(enable_loop)

    startTime = time.time()
    chainLen = dll.gen_chain_word(words, length, result, head, tail, reject, enable_loop)
    endTime = time.time()

    if chainLen < 0:
        raise WordException(chainLen)

    res = utils.bytes2str(result, chainLen)
    times = endTime - startTime
    return res, chainLen, times


def gen_chain_char(words: str, head: int, tail: int, reject: int, enable_loop: bool):
    words, length = utils.split2bytes(words)
    result = (c_char_p * 200000)()
    head = c_char(head)
    tail = c_char(tail)
    reject = c_char(reject)
    enable_loop = c_bool(enable_loop)
    startTime = time.time()
    chainLen = dll.gen_chain_char(words, length, result, head, tail, reject, enable_loop)
    endTime = time.time()

    if chainLen < 0:
        raise WordException(chainLen)

    res = utils.bytes2str(result, chainLen)
    times = endTime - startTime
    return res, chainLen, times


class WordException(Exception):
    def __init__(self, errorCode):
        self.errorCode = errorCode

    def __str__(self):
        if self.errorCode & 0xffffffff == WORD_CYCLE_EXCEPTION:
            return "存在单词环"
        elif self.errorCode & 0xffffffff == TOO_LONG_EXCEPTION:
            return "单词链过长"
        else:
            return "未知错误码" + str(self.errorCode)
