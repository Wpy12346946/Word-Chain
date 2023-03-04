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
	string word;//TODO 改成string *，避免拷贝构造
	int len;
	bool vis;

public:
	Edge(string word, bool isWeight, bool reverse);

	int getTo();

	string &getWord();

	void setVis(bool vis);

	bool isVis();
};


#endif //C_PROJECT_EDGE_H
