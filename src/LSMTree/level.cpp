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
 //TODO
}

void Level::addTuple(Tuple *tuple) {
//TODO
}

Level *Level::merge(Level *anotherLevel) {
    //TODO
    return nullptr;
}
