//
// Created by jingyusu on 3/16/22.
//

#include "fencePointer.h"

Run::Run(size_t size) : MAX_TUPLE_NUM(size) {
    tuples = vector<Tuple*>(size);
}

bool Run::isFull() {
    return tuples.size() == MAX_TUPLE_NUM;
}

Run *Run::merge(Run *anotherRun) {
    //todo
    return nullptr;
}

void Run::addTuple(Tuple *newTuple) {
    //TODO
}

Run::~Run() = default;
