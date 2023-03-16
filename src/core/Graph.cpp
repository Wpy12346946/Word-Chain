//
// Created by 57522 on 2023/3/3.
//

#include <cstring>
#include <iostream>
#include "Graph.h"

void Graph::init() {
    for (int i = 0; i < 26; i++) {
        graph[i] = vector<Edge>();
        selfCircle[i] = vector<Edge>();
        visited.push_back(false);
    }
}

map<int, vector<Edge>> Graph::makeGraph(char *words[], int len, bool isWeight, bool reverse) {
    unordered_set<string> nRepeatWord;
    for (int i = 0; i < len; i++) {
        string word(words[i]);
        if (nRepeatWord.find(word) == nRepeatWord.end()) {
            addEdge(word, isWeight, reverse);
            nRepeatWord.insert(word);
        }
    }
    return graph;
}

void Graph::addEdge(string &word, bool isWeight, bool reverse) {
    Edge edge(word, isWeight, reverse);
    // 修改入度
    if (edge.getFrom() != edge.getTo()) {
        inDegree[edge.getTo()]++;
        if (reverse) {
            graph[word[word.length() - 1] - 'a'].push_back(edge);
        } else {
            graph[word[0] - 'a'].push_back(edge);
        }
    } else {
        selfCircle[word[0] - 'a'].push_back(edge);
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
    //自环判断
    for (int i = 0; i < 26; i++) {
        if (this->selfCircle[i].size() > 1) {
            return true;
        }
    }
    while (!begin.empty()) {
        int node = begin.front();
        begin.pop();
        for (Edge &edge: graph[node]) {
            tmpInDegree[edge.getTo()]--;
            if (tmpInDegree[edge.getTo()] == 0) {
                begin.push(edge.getTo());
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
    this->selfCircle[j - 'a'].clear();
    if (reverse) {
//        for (Edge edge: this->graph[j - 'a']) {
//            if (edge.getTo() != edge.getFrom()) {
//                this->inDegree[edge.getTo()]--;
//            }
//        }
        this->inDegree[j - 'a'] = 0;//TODO 此处上方注释处的逻辑错误（应改为本行语句），但不影响正确性
        for (int i = 0; i < 26; i++) {
            graph[i].erase(remove_if(graph[i].begin(), graph[i].end(), [j](Edge &s) { return s.getTo() == j - 'a'; }),
                           graph[i].end());
        }
    } else {
        for (Edge edge: this->graph[j - 'a']) {
            this->inDegree[edge.getTo()]--;
        }
        this->graph[j - 'a'].clear();
    }
}

//获取所有链并存到res中
void Graph::findAll(int cur, vector<vector<string *>> &res, vector<string *> &chain) {
    //如果有自环边，最多只能有一个
    if (!selfCircle[cur].empty() && !selfCircle[cur][0].isVis()) {
        chain.push_back(selfCircle[cur][0].getWord());
        if (chain.size() > 1) {
            res.push_back(chain);
        }
        if (res.size() > MAX_LENGTH) {
            throw TOO_LONG_EXCEPTION;
        }
        selfCircle[cur][0].setVis(true);
        findAll(cur, res, chain);
        chain.pop_back();
        selfCircle[cur][0].setVis(false);
    }
    for (Edge &edge: graph[cur]) {
        if (!edge.isVis()) {
            chain.push_back(edge.getWord());
            if (chain.size() > 1) {
                res.push_back(chain);
            }
            if (res.size() > MAX_LENGTH) {
                throw TOO_LONG_EXCEPTION;
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
        //自环合并
        if (!selfCircle[i].empty()) {
            int idx = 0;
            for (int j = 1; j < selfCircle[i].size(); j++) {
                Edge &edge = selfCircle[i][j];
                if (selfCircle[i][idx].getLen() < edge.getLen()) {
                    idx = j;
                }
            }
            Edge edge(selfCircle[i][idx]);
            selfCircle[i].clear();
            selfCircle[i].push_back(edge);
        }
        //非自环边合并
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

void Graph::tarjan() {
    //tarjan
    int dfn[26] = {0}, low[26] = {0};
    bool inStack[26] = {false};
    stack<int> st;
    int cnt = 0;
    for (int i = 0; i < 26; i++) {
        if (dfn[i] == 0) {
            tarjanDFS(i, cnt, dfn, low, inStack, st);
        }
    }
    //删除掉非强连通分量间的所有重边
    for (int i = 0; i < 26; i++) {
        Edge *edges[26] = {nullptr};
        vector<Edge> newEdges;
        for (Edge &edge: this->graph[i]) {
            if (scc[i] != scc[edge.getTo()]) {
                int to = edge.getTo();
                if (edges[to] == nullptr || edges[to]->getLen() < edge.getLen()) {
                    edges[to] = &edge;
                }
            } else {
                newEdges.push_back(edge);
            }
        }
        for (Edge *&edge: edges) {
            if (edge != nullptr) {
                newEdges.push_back(*edge);
            }
        }
        this->graph[i] = newEdges;
    }
}

void Graph::tarjanDFS(int x, int &cnt, int dfn[], int low[], bool inStack[], stack<int> &st) {
    st.push(x);
    inStack[x] = true;
    dfn[x] = low[x] = cnt++;
    for (Edge &edge: this->graph[x]) {
        if (!dfn[edge.getTo()]) {
            tarjanDFS(edge.getTo(), cnt, dfn, low, inStack, st);
            low[x] = min(low[x], low[edge.getTo()]);
        } else if (inStack[edge.getTo()]) {
            low[x] = min(low[x], dfn[edge.getTo()]);
        }
    }
    if (dfn[x] == low[x]) {
        while (st.top() != x) {
            scc[st.top()] = low[x];
            inStack[st.top()] = false;
            st.pop();
        }
        scc[x] = x;
        inStack[st.top()] = false;
        st.pop();
    }
}

void Graph::findMax(vector<string *> &chain) {
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

void Graph::findMax(int head, vector<string *> &chain, vector<Edge *> newChain) {
    //如果有自环则一定添加
    if (!this->selfCircle[head].empty()) {
        newChain.push_back(&this->selfCircle[head][0]);
    }
    //递归遍历所有非自环边
    for (int i = 0; i < this->graph[head].size(); i++) {
        newChain.push_back(&this->graph[head][i]);
        findMax(this->graph[head][i].getTo(), chain, newChain);
        newChain.pop_back();
    }
    //无后继时链到达终点，可能为最长
    if (this->graph[head].empty()) {
        saveChain(chain, newChain);
    }
    if (!this->selfCircle[head].empty()) {
        newChain.pop_back();
    }
}

void Graph::findMax(int head, int tail, vector<string *> &chain, vector<Edge *> newChain) {
    //如果有自环则一定添加
    if (!this->selfCircle[head].empty()) {
        newChain.push_back(&this->selfCircle[head][0]);
    }
    //遇到tail时可以提前判断终止递归
    if (head == tail) {
        saveChain(chain, newChain);
        if (!this->selfCircle[head].empty()) {
            newChain.pop_back();
        }
        return;
    }
    //递归遍历所有非自环边
    for (int i = 0; i < this->graph[head].size(); i++) {
        newChain.push_back(&this->graph[head][i]);
        findMax(this->graph[head][i].getTo(), tail, chain, newChain);
        newChain.pop_back();
    }
    if (!this->selfCircle[head].empty()) {
        newChain.pop_back();
    };
}

void Graph::findMaxRecursive(vector<string *> &chain) {
    queue<int> begin;
    for (int i = 0; i < 26; i++) {
        begin.push(i);
    }
    while (!begin.empty()) {
        int node = begin.front();
        begin.pop();
        vector<Edge *> newChain;
        findMaxRecursive(node, chain, newChain);
    }
}

void Graph::findMaxRecursive(int head, vector<string *> &chain, vector<Edge *> newChain) {

    bool flag = this->visited[head];//记录是否为再次访问该节点，只有第一次访问时flag=false
    //是第一次访问时，添加所有自环边
    if (!flag) {
        this->visited[head] = true;
        for (Edge &edge: this->selfCircle[head]) {
            newChain.push_back(&edge);
        }
    }
    bool ans = true;//记录是否达到终点
    for (int i = 0; i < graph[head].size(); i++) {
        if (!this->graph[head][i].isVis()) {
            ans = false;
            break;
        }
    }
    if (ans) {
        //无后继时到达终点，可能为最长
        saveChain(chain, newChain);
    } else {
        //有后继
        for (int i = 0; i < graph[head].size(); i++) {
            if (!this->graph[head][i].isVis()) {
                newChain.push_back(&this->graph[head][i]);
                this->graph[head][i].setVis(true);
                findMaxRecursive(this->graph[head][i].getTo(), chain, newChain);
                newChain.pop_back();
                this->graph[head][i].setVis(false);
            }
        }
    }

    //第一次访问结束，弹出所有自环边并重置访问状态
    if (!flag) {
        for (int i = 0; i < this->selfCircle[head].size(); i++) {
            newChain.pop_back();
        }
        this->visited[head] = false;
    }
}

bool Graph::findMaxRecursive(int head, int tail, vector<string *> &chain, vector<Edge *> newChain) {
    //第一次读入加入所有自环
    bool flag = this->visited[head];
    if (!flag) {
        this->visited[head] = true;
        for (Edge &edge: this->selfCircle[head]) {
            newChain.push_back(&edge);
        }
    }
    //判断是否到达终点
    bool ans = true;
    bool hasNext = false;//TODO 记录是否后续的dfs中已经保存过以t结尾的链
    for (int i = 0; i < graph[head].size(); i++) {
        if (!this->graph[head][i].isVis()) {
            ans = false;
            break;
        }
    }

    if (!ans) {
        for (int i = 0; i < graph[head].size(); i++) {
            if (!this->graph[head][i].isVis()) {
                newChain.push_back(&this->graph[head][i]);
                this->graph[head][i].setVis(true);
                hasNext |= findMaxRecursive(this->graph[head][i].getTo(), tail, chain, newChain);
                newChain.pop_back();
                this->graph[head][i].setVis(false);
            }
        }
    }
    if (head == tail && !hasNext) {
        saveChain(chain, newChain);//没保存过且满足-t条件时保存
        hasNext = true;
    }
    //弹出第一次加入的所有自环
    if (!flag) {
        for (int i = 0; i < this->selfCircle[head].size(); i++) {
            newChain.pop_back();
        }
        this->visited[head] = false;
    }
    return hasNext;
}

int Graph::sum(vector<Edge *> &chain) {
    int ans = 0;
    for (Edge *edge: chain) {
        ans += edge->getLen();
    }
    return ans;
}

void Graph::saveChain(vector<string *> &chain, vector<Edge *> &edges) {
    //必须长度大于2
    if (edges.size() < 2) {
        return;
    } else if (edges.size() > MAX_LENGTH) {
        throw TOO_LONG_EXCEPTION;
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