#include "CheckMethods.h"
#include "pch.h"
#include "test.h"

void coreTest(int index) {
    std::string dataFile = "../testfile/core/data" + std::to_string(index) + ".txt";
    std::string paramFile = "../testfile/core/param" + std::to_string(index) + ".txt";
    std::vector<std::string> wordList;
    char **words = loadData(dataFile, wordList);
    Config config;
    loadConfig(paramFile, config);
    char *result[20000];
    int res;
    switch (config.api) {
        case 'n':
            res = gen_chains_all(words, wordList.size(), result);
            retCheck(res, config.ret);
            for (int i = 0; i < res; i++) {
                std::string tmp[20000];
                int cnt = 0;
                for (int j = 0; result[i][j] != '\0'; j++) {
                    if (result[i][j] == ' ') {
                        cnt++;
                    } else {
                        tmp[cnt] += result[i][j];
                    }
                }
                cnt++;
                char *tmpRes[20000];
                for (int j = 0; j < cnt; j++) {
                    tmpRes[j] = (char *) tmp[j].c_str();
                }
                if (cnt >= 0) {
                    chainCheck(tmpRes, cnt);
                    circleCheck(tmpRes, cnt);
                }
            }
            break;
        case 'w':
            res = gen_chain_word(words, wordList.size(), result, config.h, config.t, config.j, config.r);
            retCheck(res, config.ret);
            if (res >= 0) {
                chainCheck(result, res);
                headCheck(result, res, config.h);
                tailCheck(result, res, config.t);
                rejectCheck(result, res, config.j);
                if (!config.r) {
                    circleCheck(result, res);
                }
            }
            break;
        case 'c':
            res = gen_chain_char(words, wordList.size(), result, config.h, config.t, config.j, config.r);
            retCheck(res, config.ret);
            if (res >= 0) {
                chainCheck(result, res);
                headCheck(result, res, config.h);
                tailCheck(result, res, config.t);
                rejectCheck(result, res, config.j);
                if (!config.r) {
                    circleCheck(result, res);
                }
            }
            break;
    }
    delete[]words;
}

void cliTest(int index) {
    std::string configfile = "../testfile/cli/param" + std::to_string(index) + ".txt";
    std::ifstream inputfile(configfile);
    int argc = 1;
    char *argv[20];
    std::string str;
    getline(inputfile, str);
    int ans = atoi(str.c_str());
    while (getline(inputfile, str)) {
        char *arg = new char(str.size() + 1);
        strcpy_s(arg, str.size() + 1, str.c_str());
        argv[argc++] = arg;
    }
    EXPECT_EQ(ans, run(argc, argv));
}

char **loadData(std::string filepath, std::vector<std::string> &wordList) {
    std::ifstream inputFile(filepath);
    std::string str;
    while (getline(inputFile, str)) {
        std::string word;
        for (char ch: str) {
            if (isalpha(ch)) {
                ch = tolower(ch);
                word += ch;
            } else if (!word.empty()) {
                wordList.push_back(word);
                word.clear();
            }
        }
        if (!word.empty()) {
            wordList.push_back(word);
            word.clear();
        }
    }

    char **words = new char *[wordList.size()];
    for (int i = 0; i < wordList.size(); i++) {
        words[i] = const_cast<char *>(wordList[i].c_str());
    }
    inputFile.close();
    return words;
}

void loadConfig(std::string &filepath, Config &config) {
    std::ifstream inputFile(filepath);
    std::string str;
    getline(inputFile, str);
    config.api = str[0];
    config.r = str.size() > 1 && str[1] == 'r';
    getline(inputFile, str);
    config.h = (str.size() != 0 && isalpha(str[0])) ? tolower(str[0]) : '\0';
    getline(inputFile, str);
    config.t = (str.size() != 0 && isalpha(str[0])) ? tolower(str[0]) : '\0';
    getline(inputFile, str);
    config.j = (str.size() != 0 && isalpha(str[0])) ? tolower(str[0]) : '\0';
    getline(inputFile, str);
    config.ret = atoi(str.c_str());
}

TEST(test, TestMethod1) {
    for (int i = 1; i <= 22; i++) {
        coreTest(i);
    }
    for (int i = 1; i <= 14; i++) {
        cliTest(i);
    }
    for (int i = 20; i <= 50; i++) {
        cliTest(i);
    }
};