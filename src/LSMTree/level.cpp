//
// Created by jingyusu on 3/16/22.
//

#include "level.h"
#include "Run/run.h"

bool Level::isFull() {
    return runs.size() == MAX_RUN_NUM && runs[MAX_RUN_NUM - 1]->isFull();
}

Run *Level::getRun(int idx) {
    return runs[idx];
}

size_t Level::getCurrentSize() {
    return runs.size();
}

void Level::addRun(Run *run) {
    if (!isFull()) {
        runs.push_back(run);
    } else {
        throw LevelFullException();
    }
}

void Level::addTuple(Tuple *tuple) {
    if (isEmpty()) {
        Run* newRun = new Run(MAX_TUPLE_NUM_IN_RUN);
        newRun->addTuple(tuple);
        addRun(newRun);
    } else {
        //TODO: Cascade can happen here
    }
}

Run *Level::merge() {
    Run* initRun = new Run(MAX_RUN_NUM * MAX_TUPLE_NUM_IN_RUN);
    for (Run* run : runs) {
        initRun->merge(run);
    }
    clear();
    return initRun;
}

void Level::clear() {
//TODO
}

bool Level::isEmpty() {
    return runs.empty();
}
