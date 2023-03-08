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
    set<string> nRepeatWord;
    for (int i = 0; i < len; i++) {
        string word(words[i]);
        if (nRepeatWord.find(word) == nRepeatWord.end()) {
            addEdge(word, isWeight, reverse);
            nRepeatWord.insert(word);
        }
    }
    return graph;
}

void Graph::addEdge(string word, bool isWeight, bool reverse) {
    Edge edge(word, isWeight, reverse);
    // 修改入度
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

void Graph::deleteJ(char j, bool reverse) {
    if (j == '\0') {
        return;
    }
    if (reverse) {
        for (int i = 0; i < 26; i++) {
            graph[i].erase(remove_if(graph[i].begin(), graph[i].end(), [j](Edge &s) { return s.getTo() == j - 'a'; }),
                           graph[i].end());
        }
    } else {
        this->graph[j - 'a'].clear();
    }
}

//获取所有链并存到res中
void Graph::findAll(int cur, vector<vector<string>> &res, vector<string> &chain) {
    for (Edge &edge: graph[cur]) {
        if (!edge.isVis()) {
            chain.push_back(edge.getWord());
            if (chain.size() > 1) {
                res.push_back(chain);
            }
            edge.setVis(true);
            findAll(edge.getTo(), res, chain);
            chain.pop_back();
            edge.setVis(false);
        }
    }
}

void Graph::simplify() {
    // 删除非最长边，删除后保证如果有自环，则自环一定在graph[i][0]的位置
    for (int i = 0; i < 26; i++) {
        int maxIdx[26] = {0};//最大边的index
        for (int &j: maxIdx) {
            j = -1;
        }
        for (int j = 0; j < graph[i].size(); j++) {
            Edge &edge = graph[i][j];
            if (maxIdx[edge.getTo()] == -1) {
                maxIdx[edge.getTo()] = j;
            } else if (graph[i][maxIdx[edge.getTo()]].getLen() < edge.getLen()) {
                maxIdx[edge.getTo()] = j;
            }
        }
        vector<Edge> edges;
        //先尝试添加自环
        if (maxIdx[i] != -1) {
            edges.push_back(graph[i][maxIdx[i]]);
            maxIdx[i] = -1;
        }
        //然后添加其他边
        for (int j: maxIdx) {
            if (j != -1) {
                edges.push_back(graph[i][j]);
            }
        }
        graph[i].clear();
        for (Edge &edge: edges) {
            graph[i].push_back(edge);
        }
    }
}

void Graph::findMax(vector<string> &chain) {
    //获取所有起点
    queue<int> begin;
    for (int i = 0; i < 26; i++) {
        if (inDegree[i] == 0) {
            begin.push(i);
        }
    }
    while (!begin.empty()) {
        int node = begin.front();
        begin.pop();
        vector<Edge *> newChain;
        findMax(node, chain, newChain);
    }
}

void Graph::findMax(int head, vector<string> &chain, vector<Edge *> newChain) {
    //无后继时链到达终点，可能为最长
    if (this->graph[head].empty()) {
        saveChain(chain, newChain);
        return;
    }
    int i = 0;
    //如果有自环则一定添加
    if (this->graph[head][i].getTo() == head) {
        newChain.push_back(&this->graph[head][i++]);
    }
    //递归遍历所有非自环边
    while (i < this->graph[head].size()) {
        newChain.push_back(&this->graph[head][i]);
        findMax(this->graph[head][i].getTo(), chain, newChain);
        newChain.pop_back();
        i++;
    }
    if (this->graph[head][0].getTo() == head) {
        //只有一个自环边时的特判
        if (this->graph[head].size() == 1) {
            saveChain(chain, newChain);
        }
        newChain.pop_back();
    }
}

void Graph::findMax(int head, int tail, vector<string> &chain, vector<Edge *> newChain) {
    if (this->graph[head].empty()) {
        if (head == tail) {
            saveChain(chain, newChain);
        }
        return;
    }
    int i = 0;
    //如果有自环则一定添加
    if (this->graph[head][i].getTo() == head) {
        newChain.push_back(&this->graph[head][i++]);
    }
    //递归遍历所有非自环边
    while (i < this->graph[head].size()) {
        newChain.push_back(&this->graph[head][i]);
        findMax(this->graph[head][i].getTo(), tail, chain, newChain);
        newChain.pop_back();
        i++;
    }
    if (head == tail) {
        saveChain(chain, newChain);
    }
    if (this->graph[head][0].getTo() == head) {
        newChain.pop_back();
    };
}

void Graph::findMaxRecursive(vector<string> &chain) {
    queue<int> begin;
    for (int i = 0; i < 26; i++) {
        if (inDegree[i] == 0) {
            begin.push(i);
        }
    }
    while (!begin.empty()) {
        int node = begin.front();
        begin.pop();
        vector<Edge *> newChain;
        findMaxRecursive(node, chain, newChain);
    }
}

void Graph::findMaxRecursive(int head, vector<string> &chain, vector<Edge *> newChain) {
    //TODO -h -r
    //无后继时链到达终点，可能为最长
    bool ans = true;
    for (int i = 0; i < graph[head].size(); i++) {
        if (this->graph[head][i].isVis()) {
            ans = false;
            break;
        }
    }
    if (ans) {
        saveChain(chain, newChain);
        return;
    }
    //TODO 可能的优化：node节点单独存储自环边，第一次到达时加入，省略每次遍历
    for (int i = 0; i < graph[head].size(); i++) {
        if (this->graph[head][i].getTo() == head && !this->graph[head][i].isVis()) {
            newChain.push_back(&this->graph[head][i]);
            this->graph[head][i].setVis(true);
        }
    }
    for (int i = 0; i < graph[head].size(); i++) {
        if (this->graph[head][i].getTo() != head && !this->graph[head][i].isVis()) {
            newChain.push_back(&this->graph[head][i]);
            this->graph[head][i].setVis(true);
            findMaxRecursive(this->graph[head][i].getTo(), chain, newChain);
            newChain.pop_back();
            this->graph[head][i].setVis(false);
        }
    }
    saveChain(chain, newChain);
    for (int i = 0; i < graph[head].size(); i++) {
        if (this->graph[head][i].getTo() == head) {
            newChain.pop_back();
            this->graph[head][i].setVis(false);
        }
    }
}

void Graph::findMaxRecursive(int head, int tail, vector<string> &chain, vector<Edge *> newChain) {
    //TODO -h -t -r
    bool ans = true;
    for (int i = 0; i < graph[head].size(); i++) {
        if (this->graph[head][i].isVis()) {
            ans = false;
            break;
        }
    }
    if (ans) {
        if (head == tail) {
            saveChain(chain, newChain);
        }
        return;
    }
    for (int i = 0; i < graph[head].size(); i++) {
        if (this->graph[head][i].getTo() == head && !this->graph[head][i].isVis()) {
            newChain.push_back(&this->graph[head][i]);
            this->graph[head][i].setVis(true);
        }
    }
    for (int i = 0; i < graph[head].size(); i++) {
        if (this->graph[head][i].getTo() != head && !this->graph[head][i].isVis()) {
            newChain.push_back(&this->graph[head][i]);
            this->graph[head][i].setVis(true);
            findMaxRecursive(this->graph[head][i].getTo(), tail, chain, newChain);
            newChain.pop_back();
            this->graph[head][i].setVis(false);
        }
    }
    if (head == tail) {
        saveChain(chain, newChain);
    }
    for (int i = 0; i < graph[head].size(); i++) {
        if (this->graph[head][i].getTo() == head) {
            newChain.pop_back();
            this->graph[head][i].setVis(false);
        }
    }
}

int Graph::sum(vector<Edge *> &chain) {
    int ans = 0;
    for (Edge *edge: chain) {
        ans += edge->getLen();
    }
    return ans;
}

void Graph::saveChain(vector<string> &chain, vector<Edge *> &edges) {
    //必须长度大于2
    if (edges.size() < 2) {
        return;
    }
    int newChainLen = sum(edges);
    if (newChainLen > this->chainMaxLen) {
        this->chainMaxLen = newChainLen;
        chain.clear();
        for (Edge *edge: edges) {
            chain.push_back(edge->getWord());
        }
    }
}