#pragma once
#include "pch.h"
#include "CppUnitTest.h"

#include <unordered_set>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void retCheck(int res, int truth);//���ӿڷ���ֵ�Ƿ���ȷ

void chainCheck(char *results[], int len);//��鵥�������Ⱥ��������ڵ����Ƿ��������Լ��

void headCheck(char *results[], int len, char head);//����Ƿ�����-h����

void tailCheck(char *results[], int len, char tail);//����Ƿ�����-t����

void rejectCheck(char *results[], int len, char rejectCheck);//����Ƿ�����-j����

void circleCheck(char *results[], int len);//�����-r����ʱ�Ƿ������޻�Լ��