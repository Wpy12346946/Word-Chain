//
// Created by 57522 on 2023/3/3.
//

#ifndef C__PROJECT_CORE_H
#define C__PROJECT_CORE_H

#include "Graph.h"

class Core {
private:
    int genMaxchain(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop,
                    bool hasWeight);

public:
    int gen_chain_word(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop);

    int gen_chain_char(char *words[], int len, char *result[], char head, char tail, char reject, bool enable_loop);

    int gen_chains_all(char *words[], int len, char *result[]);
};


#endif //C__PROJECT_CORE_H
