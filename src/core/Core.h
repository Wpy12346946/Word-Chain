//
// Created by 57522 on 2023/3/3.
//

#ifndef C__PROJECT_CORE_H
#define C__PROJECT_CORE_H

#include "Graph.h"

const int WORD_CYCLE_EXCEPTION = 0x80000001;//有环异常
const int TOO_LONG_EXCEPTION = 0x80000002;//链过长异常
const int MAX_LENGTH = 20000;

extern "C" __declspec(dllexport) int testy();
extern "C" __declspec(dllexport) int genMaxchain(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop,
                bool hasWeight);

extern "C" __declspec(dllexport) int gen_chain_word(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop);

extern "C" __declspec(dllexport) int gen_chain_char(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop);

extern "C" __declspec(dllexport) int gen_chains_all(char *words[], int len, char *result[]);


#endif //C__PROJECT_CORE_H
