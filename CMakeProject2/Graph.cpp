//
// Created by 57522 on 2023/3/3.
//

#include <cstring>
#include <iostream>
#include "Graph.h"

void Graph::init() {
    for (int i = 0; i < 26; i++) {
        graph[i] = vector<Edge>();
    }
}

map<int, vector<Edge>> Graph::makeGraph(char *words[], int len, bool isWeight, bool reverse) {
    vector<string> nRepeatWord;
    for (int i = 0; i < len; i++) {
        string word(words[i]);
        auto it = std::find(nRepeatWord.begin(), nRepeatWord.end(), word);
        if (it == nRepeatWord.end()) {
            addEdge(word, isWeight, reverse);
            nRepeatWord.push_back(word);
        }
    }
    return graph;
}

void Graph::addEdge(string word, bool isWeight, bool reverse) {
    Edge edge(word, isWeight, reverse);
    //TODO 修改入度
    if (edge.getFrom() != edge.getTo()) {
        inDegree[edge.getTo()]++;
    }
    if (reverse) {
        graph[word[word.length() - 1] - 'a'].push_back(edge);
    } else {
        graph[word[0] - 'a'].push_back(edge);
    }
}

//判断环
bool Graph::hasCircle() {
    queue<int> begin;
    for (int i = 0; i < 26; i++) {
        if (inDegree[i] == 0) {
            begin.push(i);
        }
    }
    vector<int> tmpInDegree(inDegree, inDegree + 26);
    vector<bool> selfCircle(26);
    while (!begin.empty()) {
        int node = begin.front();
        begin.pop();
        for (Edge &edge: graph[node]) {
            if (edge.getTo() == node) {
                //自环判断
                if (selfCircle[node]) {
                    return true;
                } else {
                    selfCircle[node] = true;
                }
            } else {
                //非自环
                tmpInDegree[edge.getTo()]--;
                if (tmpInDegree[edge.getTo()] == 0) {
                    begin.push(edge.getTo());
                }
            }
        }
    }
    //判断是否有节点不为0（不为0时一定小于0，此时有环）
    return count_if(tmpInDegree.begin(), tmpInDegree.end(), [](int x) -> bool { return x != 0; }) != 0;
}

void Graph::deleteJ(char j) {

}

//获取所有链并存到res中
void Graph::findAll(int cur, vector<vector<string>> &res, vector<string> &chain) {
    for (Edge &edge: graph[cur]) {
        if (!edge.isVis()) {
            chain.push_back(edge.getWord());
            res.push_back(chain);
            edge.setVis(true);
            findAll(edge.getTo(), res, chain);
            chain.pop_back();
            edge.setVis(false);
        }
    }
}
