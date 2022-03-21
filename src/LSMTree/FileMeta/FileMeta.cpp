//
// Created by jingyusu on 3/20/22.
//

#include "FileMeta.h"

Run *FileMeta::getRun() {
    //TODO
    return nullptr;
}

bool FileMeta::isFull() const {
    return MAX_TUPLE_NUM == size;
}

void FileMeta::appendTupleToFile(Tuple *tuple) {
    if (isFull()) {
        throw RunFullException();
    } else {
        //TODO
    }
}
