//
// Created by 57522 on 2023/3/3.
//

#include <iostream>
#include "Core.h"

vector<string> results;//保存要返回给调用方的所有字符串

int Core::gen_chain_word(char *words[], int len, char *result[], char head, char tail, bool enable_loop) {
    return genMaxchain(words, len, result, head, tail, enable_loop, false);
}

int Core::gen_chain_char(char **words, int len, char **result, char head, char tail, bool enable_loop) {
    return genMaxchain(words, len, result, head, tail, enable_loop, true);
}

int Core::genMaxchain(char *words[], int len, char *result[], char head, char tail, bool enable_loop, bool hasWeight) {
    Graph graph;
    graph.init();
    results.clear();

    bool reverse = head == '\0' && tail != '\0';
    graph.makeGraph(words, len, hasWeight, reverse);
    if (!enable_loop) {
        if (graph.hasCircle()) {
            //TODO 有环异常
            return -1;
        } else {
            graph.simplify();
            //TODO -j删除

            if (head != '\0') {
                if (tail != '\0') {
                    graph.findMax(head - 'a', tail - 'a', results, vector<Edge *>());
                } else {
                    graph.findMax(head - 'a', results, vector<Edge *>());
                }
            } else if (tail != '\0') {
                graph.findMax(tail - 'a', results, vector<Edge *>());
            } else {
                graph.findMax(results);
            }
        }
    } else {
        //TODO -j删除

        if (head != '\0') {
            if (tail != '\0') {
                graph.findMaxRecursive(head - 'a', tail - 'a', results);
            } else {
                graph.findMaxRecursive(head - 'a', results);
            }
        } else if (tail != '\0') {
            graph.findMaxRecursive(tail - 'a', results);
        } else {
            graph.findMaxRecursive(results);
        }
    }
    //写入
    int res = 0;
    if (reverse) {
        for (int i = results.size() - 1; i >= 0; i--) {
            result[res++] = (char *) results[i].c_str();
        }
    } else {
        for (string &s: results) {
            result[res++] = (char *) s.c_str();
        }
    }

    return res;
}


int Core::gen_chains_all(char *words[], int len, char *result[]) {
    Graph graph;
    graph.init();
    graph.makeGraph(words, len, false, false);
    if (graph.hasCircle()) {
        //TODO 有环时异常
        return -1;
    }
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
    res = 0;
    for (string &s: results) {
        result[res++] = (char *) s.c_str();
    }

    return res;
}
