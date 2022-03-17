//
// Created by jingyusu on 3/16/22.
//

#include "run.h"

Run::Run(int size) : MAX_TUPLE_NUM(size) {
    tuples = vector<Tuple*>(size);
}

bool Run::isFull() {
    return tuples.size() == MAX_TUPLE_NUM;
}
