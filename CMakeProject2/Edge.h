//
// Created by 57522 on 2023/3/3.
//

#ifndef C_PROJECT_EDGE_H
#define C_PROJECT_EDGE_H

#include <string>
#include <algorithm>

using namespace std;

class Edge {
    int from;
    int to;
    string word;
    int len;

public:
    Edge(string word, bool isWeight, bool reverse);
};


#endif //C_PROJECT_EDGE_H
