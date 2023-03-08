﻿// src.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <vector>
#include <string>
#include "core/Graph.h"
#include "core/Core.h"

using namespace std;

int gen_chain_word(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop);

int gen_chain_char(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop);

int gen_chains_all(char *words[], int len, char *result[]);
// TODO: 在此处引用程序需要的其他标头。