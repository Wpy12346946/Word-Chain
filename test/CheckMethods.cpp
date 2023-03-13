#include "CheckMethods.h"
#include "pch.h"
#include <unordered_set>

const int WORD_CYCLE_EXCEPTION = 0x80000001;//?л???
const int TOO_LONG_EXCEPTION = 0x80000002;//????????
const int MAX_LENGTH = 20000;

void retCheck(int res, int truth) {
    EXPECT_EQ(res, truth);
}

void chainCheck(char *results[], int len) {
    EXPECT_TRUE(len <= MAX_LENGTH && len >= 0);
    std::unordered_set<std::string> wordSet;
    for (int i = 0; i < len - 1; i++) {
        char tail = results[i][strlen(results[i]) - 1];
        char head = results[i + 1][0];
        EXPECT_EQ(head, tail);
        EXPECT_TRUE(wordSet.find(results[i]) == wordSet.end());
        wordSet.insert(results[i]);
    }
}

void headCheck(char *results[], int len, char head) {
    if (isalpha(head)) {
        EXPECT_EQ(results[0][0], head);
    }
}

void tailCheck(char *results[], int len, char tail) {
    if (isalpha(tail)) {
        EXPECT_EQ(results[len - 1][strlen(results[len - 1]) - 1], tail);
    }
}

void rejectCheck(char *results[], int len, char reject) {
    if (isalpha(reject)) {
        for (int i = 0; i < len; i++) {
            EXPECT_NE(results[i][0], reject);
        }
    }
}

void circleCheck(char *results[], int len) {
    char head = results[0][0];
    char tail = results[len - 1][strlen(results[len - 1]) - 1];
    EXPECT_NE(head, tail);
}