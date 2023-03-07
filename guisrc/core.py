from ctypes import *
import time

import utils

dll = WinDLL(r".\\core.dll")


def gen_chains_all(words: str):
    words, length = utils.split2bytes(words)
    result = (c_char_p * 1000)()

    startTime = time.time()
    chainLen = dll.gen_chains_all(words, length, result)
    endTime = time.time()
    # TODO 异常处理
    if chainLen < 0:
        raise WordException

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
    # TODO 异常处理
    if chainLen < 0:
        raise WordException

    res = utils.bytes2str(result, chainLen)
    times = endTime - startTime
    return res, chainLen, times


def gen_chain_char(words: str, head: int, tail: int, reject: int, enable_loop: bool):
    words, length = utils.split2bytes(words)
    result = (c_char_p * 1000)()
    head = c_char(head)
    tail = c_char(tail)
    reject = c_char(reject)
    enable_loop = c_bool(enable_loop)
    startTime = time.time()
    chainLen = dll.gen_chain_char(words, length, result, head, tail, reject, enable_loop)
    endTime = time.time()
    # TODO 异常处理
    if chainLen < 0:
        raise WordException

    res = utils.bytes2str(result, chainLen)
    times = endTime - startTime
    return res, chainLen, times


# TODO 异常处理
class WordException(Exception):
    pass
