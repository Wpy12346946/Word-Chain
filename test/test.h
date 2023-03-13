#include "../src/core/Core.h"

//extern "C" __declspec(dllimport) int testy();
//extern "C" __declspec(dllimport) int
//gen_chain_word(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop);
//
//extern "C" __declspec(dllimport) int
//gen_chain_char(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop);
//
//extern "C" __declspec(dllimport) int gen_chains_all(char *words[], int len, char *result[]);

struct Config {
    char api;//n w c
    char h, t, j;
    bool r;
    int ret;
};

void coreTest(int index);

void cliTest(int index);

char **loadData(std::string filepath, std::vector<std::string> &wordList);

void loadConfig(std::string &filepath, Config &config);