#pragma once
#include "pch.h"
#include "CppUnitTest.h"

#include <unordered_set>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void retCheck(int res, int truth);//检查接口返回值是否正确

void chainCheck(char *results[], int len);//检查单词链长度和链中相邻单词是否满足基本约束

void headCheck(char *results[], int len, char head);//检查是否满足-h参数

void tailCheck(char *results[], int len, char tail);//检查是否满足-t参数

void rejectCheck(char *results[], int len, char rejectCheck);//检查是否满足-j参数

void circleCheck(char *results[], int len);//检查无-r参数时是否满足无环约束