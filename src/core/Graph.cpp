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

void Graph::makeGraph(char *words[], int len, bool isWeight, bool reverse) {
    this->isChar = isWeight;
    unordered_set<string> nRepeatWord;
    for (int i = 0; i < len; i++) {
        string word(words[i]);
        if (nRepeatWord.find(word) == nRepeatWord.end()) {
            addEdge(word, isWeight, reverse);
            nRepeatWord.insert(word);
        }
    }
}

void Graph::addEdge(string &word, bool isWeight, bool reverse) {
    Edge edge(word, isWeight, reverse);
    // 修改入度
    if (edge.from != edge.to) {
        inDegree[edge.to]++;
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
            tmpInDegree[edge.to]--;
            if (tmpInDegree[edge.to] == 0) {
                begin.push(edge.to);
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
        this->inDegree[j - 'a'] = 0;// 此处上方注释处的逻辑错误（应改为本行语句），但不影响正确性
        for (int i = 0; i < 26; i++) {
            graph[i].erase(remove_if(graph[i].begin(), graph[i].end(), [j](Edge &s) { return s.to == j - 'a'; }),
                           graph[i].end());
        }
    } else {
        for (Edge &edge: this->graph[j - 'a']) {
            this->inDegree[edge.to]--;
        }
        this->graph[j - 'a'].clear();
    }
}

//获取所有链并存到res中
void Graph::findAll(int cur, vector<vector<string *>> &res, vector<string *> &chain) {
    //如果有自环边，最多只能有一个
    if (!selfCircle[cur].empty() && !selfCircle[cur][0].vis) {
        chain.push_back(&selfCircle[cur][0].word);
        if (chain.size() > 1) {
            res.push_back(chain);
        }
        if (res.size() > MAX_LENGTH) {
            throw TOO_LONG_EXCEPTION;
        }
        selfCircle[cur][0].vis = true;
        findAll(cur, res, chain);
        chain.pop_back();
        selfCircle[cur][0].vis = false;
    }
    for (Edge &edge: graph[cur]) {
        if (!edge.vis) {
            chain.push_back(&edge.word);
            if (chain.size() > 1) {
                res.push_back(chain);
            }
            if (res.size() > MAX_LENGTH) {
                throw TOO_LONG_EXCEPTION;
            }
            edge.vis = true;
            findAll(edge.to, res, chain);
            chain.pop_back();
            edge.vis = false;
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
                if (selfCircle[i][idx].len < edge.len) {
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
            if (maxIdx[edge.to] == -1) {
                maxIdx[edge.to] = j;
            } else if (graph[i][maxIdx[edge.to]].len < edge.len) {
                maxIdx[edge.to] = j;
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
    int dfn[26], low[26];
    for (int &i: dfn) {
        i = -1;
    }
    bool inStack[26] = {false};
    stack<int> st;
    int cnt = 0;
    this->num = 0;
    for (int i = 0; i < 26; i++) {
        if (dfn[i] == -1) {
            tarjanDFS(i, cnt, dfn, low, inStack, st);
        }
    }
    //删除掉非强连通分量间的所有重边，并储存在connects里
    for (int i = 0; i < 26; i++) {
        vector<Edge> newEdges;
        Edge *edges[26] = {nullptr};//非强连通分量之间的边
        for (Edge &edge: this->graph[i]) {
            if (scc[i] != scc[edge.to]) {
                int to = edge.to;
                if (edges[to] == nullptr || edges[to]->len < edge.len) {
                    edges[to] = &edge;//保存非强连通分量边
                }
            } else {
                newEdges.push_back(edge);
            }
        }
        for (Edge *edge: edges) {
            if (edge != nullptr)
                this->connects[i].push_back(*edge);
        }
        this->graph[i] = newEdges;
    }

    for (int i = 0; i < this->num; i++) {
        this->outDegree[i] = 0;
        for (int j = 0; j < this->num; j++) {
            this->scc_connects[i][j] = false;
        }
    }

    for (int i = 0; i < 26; i++) {
        for (Edge &edge: this->connects[i]) {
            this->scc_connects[scc[i]][scc[edge.to]] = true;
        }
    }
    for (int i = 0; i < this->num; i++) {
        for (int j = 0; j < this->num; j++) {
            if (this->scc_connects[i][j] && i != j) {
                this->outDegree[i]++;//inDegree表示第i个强连通分量的出度
            }
        }
    }
}

void Graph::tarjanDFS(int x, int &cnt, int dfn[], int low[], bool inStack[], stack<int> &st) {
    st.push(x);
    inStack[x] = true;
    dfn[x] = low[x] = cnt++;
    for (Edge &edge: this->graph[x]) {
        if (dfn[edge.to] == -1) {
            tarjanDFS(edge.to, cnt, dfn, low, inStack, st);
            low[x] = min(low[x], low[edge.to]);
        } else if (inStack[edge.to]) {
            low[x] = min(low[x], dfn[edge.to]);
        }
    }
    if (dfn[x] == low[x]) {
        while (st.top() != x) {
            this->scc_rev[this->num].push_back(st.top());
            scc[st.top()] = this->num;
            inStack[st.top()] = false;
            st.pop();
        }
        this->scc_rev[this->num].push_back(x);
        scc[x] = this->num++;
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
        this->charLen += newChain.back()->len;
    }
    //递归遍历所有非自环边
    for (Edge &edge: this->graph[head]) {
        newChain.push_back(&edge);
        this->charLen += newChain.back()->len;
        findMax(edge.to, chain, newChain);
        this->charLen -= newChain.back()->len;
        newChain.pop_back();
    }
    //无后继时链到达终点，可能为最长
    if (this->graph[head].empty()) {
        saveChain(chain, newChain);
    }
    if (!this->selfCircle[head].empty()) {
        this->charLen -= newChain.back()->len;
        newChain.pop_back();
    }
}

void Graph::findMax(int head, int tail, vector<string *> &chain, vector<Edge *> newChain) {
    //如果有自环则一定添加
    if (!this->selfCircle[head].empty()) {
        newChain.push_back(&this->selfCircle[head][0]);
        this->charLen += newChain.back()->len;
    }
    //遇到tail时可以提前判断终止递归
    if (head == tail) {
        saveChain(chain, newChain);
        if (!this->selfCircle[head].empty()) {
            this->charLen -= newChain.back()->len;
            newChain.pop_back();
        }
        return;
    }
    //递归遍历所有非自环边
    for (int i = 0; i < this->graph[head].size(); i++) {
        newChain.push_back(&this->graph[head][i]);
        this->charLen += newChain.back()->len;
        findMax(this->graph[head][i].to, tail, chain, newChain);
        this->charLen -= newChain.back()->len;
        newChain.pop_back();
    }
    if (!this->selfCircle[head].empty()) {
        this->charLen -= newChain.back()->len;
        newChain.pop_back();
    };
}

//找到强连通分量内部的最长链
void Graph::selfLongest(int cur, int maxLen[], vector<string *> maxChain[], vector<Edge *> &newChain) {
    //cur 当前所在节点
    //maxLen 以i结尾的最长距离
    //maxChain 以i结尾的最长链
    bool flag = this->visited[cur];//记录是否为再次访问该节点，只有第一次访问时flag=false
    //是第一次访问时，添加所有自环边
    if (!flag) {
        this->visited[cur] = true;
        for (Edge &edge: this->selfCircle[cur]) {
            this->charLen += edge.len;
            newChain.push_back(&edge);
        }
    }
    for (Edge &edge: graph[cur]) {
        //graph中的一定边一定属于同一强连通分量
        if (!edge.vis) {
            newChain.push_back(&edge);
            this->charLen += newChain.back()->len;
            edge.vis = true;
            selfLongest(edge.to, maxLen, maxChain, newChain);
            this->charLen -= newChain.back()->len;
            newChain.pop_back();
            edge.vis = false;
        }
    }
    saveChain(maxLen[cur], maxChain[cur], newChain);

    //第一次访问结束，弹出所有自环边并重置访问状态
    if (!flag) {
        for (int i = 0; i < this->selfCircle[cur].size(); i++) {
            this->charLen -= newChain.back()->len;
            newChain.pop_back();
        }
        this->visited[cur] = false;
    }
}

void Graph::topoSort(unordered_set<int> &contains, queue<int> &begin) {
    // contains: 可能经过的所有强连通分量集合
    //begin: 返回的顺序集合
    queue<int> tmp;
    for (int i = 0; i < this->num; i++) {
        if (this->outDegree[i] == 0 && contains.find(i) != contains.end())
            tmp.push(i);
    }
    while (!tmp.empty()) {
        int node = tmp.front();
        begin.push(node);
        tmp.pop();
        for (int i = 0; i < this->num; i++) {
            if (scc_connects[i][node]) {
                this->outDegree[i]--;
                if (this->outDegree[i] == 0 && contains.find(i) != contains.end())
                    tmp.push(i);
            }
        }
    }
}

void Graph::findMaxRecursive(vector<string *> &chain) {
    //拓扑排序
    queue<int> begin;//强连通分量集合的拓扑序，从最后往前遍历
    unordered_set<int> contains;
    for (int i = 0; i < 26; i++) {
        contains.insert(i);
    }
    topoSort(contains, begin);
    //计算每个强连通分量的最长
    vector<string *> dp[26][26];//从i到j最长距离对应的单词链
    int dpMaxLen[26][26] = {{0}};//从i到j的最长距离
    vector<Edge *> newChain;
    queue<int> q(begin);
    while (!q.empty()) {
        int scc_node = q.front();
        q.pop();
        for (int node: scc_rev[scc_node])
            selfLongest(node, dpMaxLen[node], dp[node], newChain);
    }

    //根据拓扑序从后向前dp
    int ans[26] = {0};//记录以i为起点的最长路径
    bool oneChain[26] = {false};//判断以i为起点的链是否为单链
    vector<int> start_end_list[26];//ans对应的start-end节点路径
    while (!begin.empty()) {
        int cur_scc = begin.front();//当前强连通分量
        begin.pop();
        for (int &start: this->scc_rev[cur_scc]) {
            ans[start] = 0;//临时记录以start为链起点时的路径总长度
            start_end_list[start] = {start};
            for (int &end: this->scc_rev[cur_scc]) {
                //start-end 当前强连通分量集合的起点-终点
                if (dpMaxLen[start][end] > ans[start]) {
                    ans[start] = dpMaxLen[start][end];
                    start_end_list[start] = {start, end};
                }
                for (Edge &edge: this->connects[end]) {
                    if (dpMaxLen[start][end] + ans[edge.to] + edge.len > ans[start]) {
                        // 判断单词链长大于1,此时一定有start=end
                        if (dpMaxLen[start][end] == 0 && ans[edge.to] == 0) {
                            //如果没有指向该scc的强连通分量，则说明会形成单链
                            bool hasFrontFlag = false;//是否是拓扑序中最靠前的
                            for (int i = 0; i < this->num; i++)
                                hasFrontFlag |= this->scc_connects[i][scc[start]];
                            if (!hasFrontFlag) continue;//没有更靠前的强连通分量，如果替换就会出现单链
                        }
                        ans[start] = dpMaxLen[start][end] + ans[edge.to] + edge.len;
                        start_end_list[start] = {start, end};
                        start_end_list[start].insert(start_end_list[start].end(), start_end_list[edge.to].begin(),
                                                     start_end_list[edge.to].end());
                    }
                }
            }
        }
    }
    int maxNode = 0;
    for (int i = 1; i < 26; i++) {
        if (ans[i] > ans[maxNode])
            maxNode = i;
    }
    saveChain(start_end_list[maxNode], dp, chain);
}

void Graph::findMaxRecursive(int head, vector<string *> &chain) {
    //拓扑排序
    unordered_set<int> contains;//h可能经过的所有强连通分量集合
    queue<int> q;
    q.push(this->scc[head]);
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        contains.insert(node);
        for (int i = 0; i < this->num; i++) {
            if (this->scc_connects[node][i])
                q.push(i);
        }
    }

    queue<int> begin;//强连通分量集合的拓扑序，从最后往前遍历
    topoSort(contains, begin);
    //计算每个强连通分量的最长
    vector<string *> dp[26][26];//从i到j最长距离对应的单词链
    int dpMaxLen[26][26] = {{0}};//从i到j的最长距离
    vector<Edge *> newChain;
    q = begin;
    while (!q.empty()) {
        int scc_node = q.front();
        q.pop();
        if (scc_node == scc[head])
            selfLongest(head, dpMaxLen[head], dp[head], newChain);
        else
            for (int node: scc_rev[scc_node])
                selfLongest(node, dpMaxLen[node], dp[node], newChain);
    }

    //根据拓扑序从后向前dp
    int ans[26] = {0};//记录以i为起点的最长路径
    vector<int> start_end_list[26];//ans对应的start-end节点路径
    while (!begin.empty()) {
        int cur_scc = begin.front();//当前强连通分量
        begin.pop();
        if (cur_scc == scc[head]) {
            int start = head;
            ans[start] = 0;//临时记录以start为链起点时的路径总长度
            start_end_list[start] = {start};
            for (int &end: this->scc_rev[cur_scc]) {
                //start-end 当前强连通分量集合的起点-终点
                if (dpMaxLen[start][end] > ans[start]) {
                    ans[start] = dpMaxLen[start][end];
                    start_end_list[start] = {start, end};
                }
                for (Edge &edge: this->connects[end]) {
                    if (dpMaxLen[start][end] + ans[edge.to] + edge.len > ans[start]) {
                        if (dpMaxLen[start][end] == 0 && ans[edge.to] == 0) continue;//单词链长必须大于1
                        ans[start] = dpMaxLen[start][end] + ans[edge.to] + edge.len;
                        start_end_list[start] = {start, end};
                        start_end_list[start].insert(start_end_list[start].end(), start_end_list[edge.to].begin(),
                                                     start_end_list[edge.to].end());
                    }
                }
            }
        } else {
            for (int &start: this->scc_rev[cur_scc]) {
                ans[start] = 0;//临时记录以start为链起点时的路径总长度
                start_end_list[start] = {start};
                for (int &end: this->scc_rev[cur_scc]) {
                    //start-end 当前强连通分量集合的起点-终点
                    if (dpMaxLen[start][end] > ans[start]) {
                        ans[start] = dpMaxLen[start][end];
                        start_end_list[start] = {start, end};
                    }
                    for (Edge &edge: this->connects[end]) {
                        if (dpMaxLen[start][end] + ans[edge.to] + edge.len > ans[start]) {
                            ans[start] = dpMaxLen[start][end] + ans[edge.to] + edge.len;
                            start_end_list[start] = {start, end};
                            start_end_list[start].insert(start_end_list[start].end(), start_end_list[edge.to].begin(),
                                                         start_end_list[edge.to].end());
                        }
                    }
                }
            }
        }
    }
    saveChain(start_end_list[head], dp, chain);
}

void Graph::findMaxRecursive(int head, int tail, vector<string *> &chain) {
    //拓扑排序
    //正反遍历取交集获取所有-h-t路径可能经过的强连通集
    unordered_set<int> contains1;
    queue<int> q;
    q.push(this->scc[head]);
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        contains1.insert(node);
        for (int i = 0; i < this->num; i++) {
            if (this->scc_connects[node][i])
                q.push(i);
        }
    }

    unordered_set<int> contains2;
    q.push(this->scc[tail]);
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        contains2.insert(node);
        for (int i = 0; i < this->num; i++) {
            if (this->scc_connects[i][node])
                q.push(i);
        }
    }
    unordered_set<int> contains;
    for (int node: contains1) {
        if (contains2.find(node) != contains2.end()) {
            contains.insert(node);
        }
    }

    for (int node = 0; node < this->num; node++) {
        if (contains.find(node) == contains.end()) {
            for (int i = 0; i < this->num; i++) {
                if (this->scc_connects[i][node]) {
                    this->outDegree[i]--;
                }
            }
        }
    }

    queue<int> begin;//强连通分量集合的拓扑序，从最后往前遍历
    topoSort(contains, begin);
    //计算每个强连通分量的最长
    vector<string *> dp[26][26];//从i到j最长距离对应的单词链
    int dpMaxLen[26][26] = {{0}};//从i到j的最长距离
    vector<Edge *> newChain;
    q = begin;
    while (!q.empty()) {
        int scc_node = q.front();
        q.pop();
        if (scc_node == scc[head])
            selfLongest(head, dpMaxLen[head], dp[head], newChain);
        else
            for (int node: scc_rev[scc_node])
                selfLongest(node, dpMaxLen[node], dp[node], newChain);
    }

    //根据拓扑序从后向前dp
    int ans[26] = {0};//记录以i为起点的最长路径
    vector<int> start_end_list[26];//ans对应的start-end节点路径
    while (!begin.empty()) {
        int cur_scc = begin.front();//当前强连通分量
        begin.pop();
        if (cur_scc == scc[head] && cur_scc == scc[tail]) {
            if (dp[head][tail].size() == 1) continue;// 判断单词链长大于1
            ans[head] = dpMaxLen[head][tail];
            start_end_list[head] = {head, tail};
        } else if (cur_scc == scc[head]) {
            int start = head;
            ans[start] = 0;//临时记录以start为链起点时的路径总长度
            start_end_list[start] = {start};
            for (int &end: this->scc_rev[cur_scc]) {
                //start-end 当前强连通分量集合的起点-终点
                for (Edge &edge: this->connects[end]) {
                    if ((ans[edge.to] != 0 || edge.to == tail) &&
                        dpMaxLen[start][end] + ans[edge.to] + edge.len > ans[start]) {
                        ans[start] = dpMaxLen[start][end] + ans[edge.to] + edge.len;
                        start_end_list[start] = {start, end};
                        start_end_list[start].insert(start_end_list[start].end(), start_end_list[edge.to].begin(),
                                                     start_end_list[edge.to].end());
                    }
                }
            }
        } else if (cur_scc == scc[tail]) {
            for (int &start: this->scc_rev[cur_scc]) {
                ans[start] = dpMaxLen[start][tail];
                start_end_list[start] = {start, tail};
            }
        } else {
            for (int &start: this->scc_rev[cur_scc]) {
                ans[start] = 0;//临时记录以start为链起点时的路径总长度
                start_end_list[start] = {start};
                for (int &end: this->scc_rev[cur_scc]) {
                    //start-end 当前强连通分量集合的起点-终点
                    for (Edge &edge: this->connects[end]) {
                        if ((ans[edge.to] != 0 || edge.to == tail) &&
                            dpMaxLen[start][end] + ans[edge.to] + edge.len > ans[start]) {
                            ans[start] = dpMaxLen[start][end] + ans[edge.to] + edge.len;
                            start_end_list[start] = {start, end};
                            start_end_list[start].insert(start_end_list[start].end(), start_end_list[edge.to].begin(),
                                                         start_end_list[edge.to].end());
                        }
                    }
                }
            }
        }
    }
    saveChain(start_end_list[head], dp, chain);
}

void Graph::saveChain(vector<string *> &chain, vector<Edge *> &edges) {
    //必须长度大于2
    if (edges.size() < 2) {
        return;
    } else if (edges.size() > MAX_LENGTH) {
        throw TOO_LONG_EXCEPTION;
    }
    int newChainLen = this->isChar ? this->charLen : edges.size();
    if (newChainLen > this->chainMaxLen) {
        this->chainMaxLen = newChainLen;
        chain.clear();
        for (Edge *edge: edges) {
            chain.push_back(&edge->word);
        }
    }
}

void Graph::saveChain(int &maxLen, vector<string *> &chain, vector<Edge *> &edges) {
    int newChainLen = this->isChar ? this->charLen : (int) edges.size();
    if (newChainLen > maxLen) {
        maxLen = newChainLen;
        chain.clear();
        for (Edge *edge: edges) {
            chain.push_back(&edge->word);
        }
    }
}

void Graph::saveChain(vector<int> &start_end_list, vector<string *> dp[][26], vector<string *> &chain) {
    if (start_end_list.empty())
        return;
    for (int i = 0; i < start_end_list.size() - 1; i++) {
        int start = start_end_list[i];
        int end = start_end_list[i + 1];
        if (scc[start] == scc[end])
            chain.insert(chain.end(), dp[start][end].begin(), dp[start][end].end());
        else {
            for (Edge &edge: this->connects[start]) {
                if (edge.to == end) {
                    chain.push_back(&edge.word);
                    break;
                }
            }
        }
        if (chain.size() > MAX_LENGTH)
            throw TOO_LONG_EXCEPTION;
    }
    if (chain.size() == 1)
        chain.clear();
}