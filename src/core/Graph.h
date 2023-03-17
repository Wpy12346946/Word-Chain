#ifndef C__PROJECT_GRAPH_H
#define C__PROJECT_GRAPH_H

#include <map>
#include "Edge.h"
#include "Core.h"
#include <vector>
#include <set>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <string>
#include <stack>

class Graph {
    vector<Edge> graph[26];//储存非自环边
    vector<Edge> selfCircle[26];//储存自环边;
    vector<bool> visited;
    int scc[26];//强连通分量所属集合
    unordered_set<int>firsts;//可以为起点的强连通分量
    int inDegree[26] = {0};//记录每个点（不含自环）的入度
    int chainMaxLen = 0;//最大链长度
    int charLen=0;//当前链长度
    bool isChar;//字符/单词?
public:
    void init();

    void makeGraph(char *words[], int len, bool isWeight, bool reverse);

    void addEdge(string &word, bool isWeight, bool reverse);

    bool hasCircle();

    void simplify();// 所有边只保留最大的

    void tarjan();//强连通分量

    void tarjanDFS(int x, int &cnt, int dfn[], int low[], bool inStack[], stack<int> &st);

    void deleteJ(char j, bool reverse);//TODO 删除-j的边（如果参数为-t -j时如何处理最好）

    void findAll(int cur, vector<vector<string *>> &res, vector<string *> &chain);

    void findMax(vector<string *> &chain);

    void findMax(int head, vector<string *> &chain, vector<Edge *> newChain);

    void findMax(int head, int tail, vector<string *> &chain, vector<Edge *> newChain);

    void findMaxRecursive(vector<string *> &chain);

    void findMaxRecursive(int head, vector<string *> &chain, vector<Edge *> &newChain);

    bool findMaxRecursive(int head, int tail, vector<string *> &chain, vector<Edge *> &newChain);

    void saveChain(vector<string *> &chain, vector<Edge *> &edges);//判断并决定是否更新（edges满足所有参数的条件）
};


#endif //C__PROJECT_GRAPH_H
