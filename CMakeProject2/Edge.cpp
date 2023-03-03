//
// Created by 57522 on 2023/3/3.
//
#include "Edge.h"

Edge::Edge(string word, bool isWeight, bool reverse) {
    this->word = word;
    if (reverse) {
        this->from = word[word.length() - 1] - 'a';
        this->to = word[0] - 'a';
    } else {
        this->from = word[0] - 'a';
        this->to = word[word.length() - 1] - 'a';
    }
    this->len = isWeight ? word.length() : 1;
}
