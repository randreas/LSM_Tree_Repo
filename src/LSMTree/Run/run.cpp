//
// Created by jingyusu on 3/16/22.
//

#include "run.h"

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


FileMeta *Run::createFileMetaFromRun() {
    //TODO
    return nullptr;
}

Run::~Run() {
    while (!tuples.empty()) {
        Tuple* tuple = tuples.back();
        tuples.pop_back();
        delete tuple;
    }
}
