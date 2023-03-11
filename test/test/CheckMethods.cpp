#include "CheckMethods.h"

const int WORD_CYCLE_EXCEPTION = 0x80000001;//有环异常
const int TOO_LONG_EXCEPTION = 0x80000002;//链过长异常
const int MAX_LENGTH = 20000;

void retCheck(int res, int truth) {
	Assert::AreEqual(res, truth);
}

void chainCheck(char *results[], int len) {
	Assert::IsTrue(len <= MAX_LENGTH && len >= 0);
	std::unordered_set<std::string>wordSet;
	for (int i = 0; i < len - 1; i++) {
		char tail = results[i][strlen(results[i]) - 1];
		char head = results[i + 1][0];
		Assert::AreEqual(head, tail);
		Assert::IsTrue(wordSet.find(results[i]) == wordSet.end());
		wordSet.insert(results[i]);
	}
}

void headCheck(char *results[], int len, char head) {
	if (isalpha(head)) {
		Assert::AreEqual(results[0][0], head);
	}
}

void tailCheck(char *results[], int len, char tail) {
	if (isalpha(tail)) {
		Assert::AreEqual(results[len - 1][strlen(results[len - 1]) - 1], tail);
	}
}

void rejectCheck(char *results[], int len, char reject) {
	if (isalpha(reject)) {
		for (int i = 0; i < len; i++) {
			Assert::AreNotEqual(results[i][0], reject);
		}
	}
}

void circleCheck(char *results[], int len) {
	char head = results[0][0];
	char tail = results[len - 1][strlen(results[len - 1]) - 1];
	Assert::AreNotEqual(head, tail);
}