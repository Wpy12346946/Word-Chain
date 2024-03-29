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
    vector<Edge> connects[26];//不同集合的强连通分量的边
    int scc[26];//强连通分量所属集合
    vector<int> scc_rev[26];//每个强连通分量集合包含的点
    bool scc_connects[26][26];//强连通分量集合的连接关系i指向j
    int outDegree[26];//第i个强连通分量的出度
    int inDegree[26] = {0};//记录每个点（不含自环）的入度
    int chainMaxLen = 0;//最大链长度
    int charLen = 0;//当前链长度
    bool isChar;//字符/单词?
    int num = 0;//强连通分量集合数
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

    void findMaxRecursive(int head, vector<string *> &chain);

    void findMaxRecursive(int head, int tail, vector<string *> &chain);

    void saveChain(vector<string *> &chain, vector<Edge *> &edges);//判断并决定是否更新（edges满足所有参数的条件）

    void selfLongest(int cur, int maxLen[], vector<string *> maxChain[], vector<Edge *> &newChain);

    void topoSort(unordered_set<int> &contains, queue<int> &begin);

    void saveChain(int &maxLen, vector<string *> &chain, vector<Edge *> &edges);

    void saveChain(vector<int> &start_end_list, vector<string *> dp[][26], vector<string *> &chain);
};


#endif //C__PROJECT_GRAPH_H
