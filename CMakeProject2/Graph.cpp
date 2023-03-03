//
// Created by 57522 on 2023/3/3.
//

#include <cstring>
#include "Graph.h"

void Graph::init() {
    for (int i = 0; i < 26; i++) {
        graph[i] = vector<Edge>();
    }
}

void Graph::makeGraph(char *words[], bool isWeight, bool reverse) {
    vector<string> nRepeatWord;
    for (int i = 0; strcmp(words[i], "") != 0; i++) {
        string word(words[i]);
        auto it = std::find(nRepeatWord.begin(), nRepeatWord.end(), word);
        if (it == nRepeatWord.end()) {
            addEdge(word, isWeight, reverse);
            nRepeatWord.push_back(word);
        }
    }
}

void Graph::addEdge(string word, bool isWeight, bool reverse) {
    Edge edge(word, isWeight, reverse);
    if (reverse) {
        graph[word[word.length() - 1] - 'a'].push_back(edge);
    } else {
        graph[word[0] - 'a'].push_back(edge);
    }
}
