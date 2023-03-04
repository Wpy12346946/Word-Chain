//
// Created by 57522 on 2023/3/3.
//

#include <cstring>
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
	if (reverse) {
		graph[word[word.length() - 1] - 'a'].push_back(edge);
	} else {
		graph[word[0] - 'a'].push_back(edge);
	}
}

bool Graph::hasCircle() {
	return true;
}

void Graph::deleteJ(char j) {

}

//获取所有链并存到res中
void Graph::findAll(int cur, vector<vector<string>> &res, vector<string> &chain) {
	for (Edge edge : graph[cur]) {
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
