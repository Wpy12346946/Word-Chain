//
// Created by 57522 on 2023/3/3.
//

#include <iostream>
#include "Core.h"

vector<string> stores;//保存要返回给调用方的所有字符串
vector<string*> results;
int testy() {
    return 1;
}

int gen_chain_word(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop) {
    int res;
    try {
        res = genMaxchain(words, len, result, head, tail, reject, enable_loop, false);
    } catch (int e) {
        return e;
    }
    return res;
}

int gen_chain_char(char **words, int len, char **result, char head, char tail, char reject, bool enable_loop) {
    int res;
    try {
        res = genMaxchain(words, len, result, head, tail, reject, enable_loop, true);
    } catch (int e) {
        return e;
    }
    return res;
}

int genMaxchain(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop,
                bool hasWeight) {
    Graph graph;
    graph.init();
    results.clear();
    stores.clear();

    bool reverse = head == '\0' && tail != '\0';
    graph.makeGraph(words, len, hasWeight, reverse);
    if (!enable_loop) {
        if (graph.hasCircle()) {
            return WORD_CYCLE_EXCEPTION;
        } else {
            // -j删除
            graph.deleteJ(reject, reverse);
            graph.simplify();

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
        // -j删除
        graph.deleteJ(reject, reverse);

        graph.tarjan();
        vector<Edge *> chain;
        if (head != '\0') {
            if (tail != '\0') {
                graph.findMaxRecursive(head - 'a', tail - 'a', results, chain);
            } else {
                graph.findMaxRecursive(head - 'a', results, chain);
            }
        } else if (tail != '\0') {
            graph.findMaxRecursive(tail - 'a', results, chain);
        } else {
            graph.findMaxRecursive(results);
        }
    }
    //写入
    int res = 0;
    if (reverse) {
        for (int i = results.size() - 1; i >= 0; i--) {
            stores.push_back(*results[i]);
            result[res++] = (char *) stores.back().c_str();
        }
    } else {
        for (string *s: results) {
            stores.push_back(*s);
        }
        for (string &s:stores){
            result[res++] = (char *) s.c_str();
        }
    }

    return res;
}


int gen_chains_all(char *words[], int len, char *result[]) {
    Graph graph;
    graph.init();
    graph.makeGraph(words, len, false, false);
    if (graph.hasCircle()) {
        return WORD_CYCLE_EXCEPTION;
    }
    vector<vector<string*>> allChains;
    for (int i = 0; i < 26; i++) {
        vector<string*> chain;
        try {
            graph.findAll(i, allChains, chain);
        } catch (int e) {
            return e;
        }
    }

    results.clear();
    stores.clear();
    int res = 0;
    for (vector<string*> &chain: allChains) {
        string s;
        stores.emplace_back();
        for (string *word: chain) {
            stores[res] += *word;
            stores[res] += " ";
        }
        stores[res].pop_back();
        res++;
    }
    res = 0;
    for (string &s: stores) {
        result[res++] = (char *) s.c_str();
    }

    return res;
}
