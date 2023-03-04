//
// Created by 57522 on 2023/3/3.
//

#ifndef C__PROJECT_GRAPH_H
#define C__PROJECT_GRAPH_H

#include <map>
#include "Edge.h"
#include <vector>
#include <algorithm>
#include <string>

class Graph {
	map<int, vector<Edge>> graph;
public:
	void init();

	map<int, vector<Edge>> makeGraph(char *words[], int len, bool isWeight, bool reverse);

	void addEdge(string word, bool isWeight, bool reverse);

	bool hasCircle();

	void deleteJ(char j);

	void findAll(int cur, vector<vector<string>> &res, vector<string> &chain);
};


#endif //C__PROJECT_GRAPH_H
