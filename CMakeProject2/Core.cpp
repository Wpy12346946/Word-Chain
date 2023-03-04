//
// Created by 57522 on 2023/3/3.
//

#include <iostream>
#include "Core.h"

vector<string> results;//保存要返回给调用方的所有字符串

int Core::gen_chain_word(char *words[], int len, char *result[], char head, char tail, bool enable_loop) {
    return 0;
}

int Core::gen_chains_all(char *words[], int len, char *result[]) {
    Graph graph;
    graph.init();
    graph.makeGraph(words, len, false, false);
    vector<vector<string>> allChains;//TODO 考虑用指针来减少string拷贝的时间
    for (int i = 0; i < 26; i++) {
        vector<string> chain;
        graph.findAll(i, allChains, chain);
    }

    //TODO 总链数过多时异常
    results.clear();
    int res = 0;
    for (vector<string> &chain: allChains) {
        string s;
        results.emplace_back();
        for (string &word: chain) {
            results[res] += word;
            results[res] += " ";
        }
        results[res].pop_back();
        res++;
    }
    res=0;
    for(string &s:results){
        result[res++]=(char *)s.c_str();
    }

    return res;
}