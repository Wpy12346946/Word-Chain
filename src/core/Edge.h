//
// Created by 57522 on 2023/3/3.
//

#ifndef C_PROJECT_EDGE_H
#define C_PROJECT_EDGE_H

#include <string>
#include <algorithm>

using namespace std;

class Edge {
public:
    int from;
    int to;
    string word;
    int len;
    bool vis;//是否被访问过

    Edge(string &word, bool isWeight, bool reverse);
};


#endif //C_PROJECT_EDGE_H
