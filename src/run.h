#ifndef TEST_RUN_H
#define TEST_RUN_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <vector>
#include <string>

int run(int argc, char *argv[]);

//extern "C" __declspec(dllimport) int
//gen_chain_word(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop);
//
//extern "C" __declspec(dllimport) int
//gen_chain_char(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop);
//
//extern "C" __declspec(dllimport) int gen_chains_all(char *words[], int len, char *result[]);

#include "core/Core.h"
#include "Error.h"

#endif //TEST_RUN_H
