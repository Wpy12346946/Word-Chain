//
// Created by 57522 on 2023/3/3.
//
#include "Edge.h"

Edge::Edge(string &word, bool isWeight, bool reverse) {
    this->word = word;
    if (reverse) {
        this->from = word[word.length() - 1] - 'a';
        this->to = word[0] - 'a';
    } else {
        this->from = word[0] - 'a';
        this->to = word[word.length() - 1] - 'a';
    }
    this->len = isWeight ? word.length() : 1;
    this->vis= false;
}

int Edge::getTo() {
    return to;
}

int Edge::getFrom() {
    return from;
}

string *Edge::getWord() {
    return &word;
}

void Edge::setVis(bool vis) {
    this->vis = vis;
}

bool Edge::isVis() {
    return this->vis;
}

int Edge::getLen() {
    return this->len;
}
