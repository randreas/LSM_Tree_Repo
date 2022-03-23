//
// Created by jingyusu on 3/16/22.
//

#include "level.h"
#include "Run/run.h"
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fstream>

bool Level::isFull() {
    return dataBlocks.size() == MAX_RUN_NUM && dataBlocks[MAX_RUN_NUM - 1]->isFull();
}

Run *Level::getRun(int idx) {
    return dataBlocks[idx]->getRun();
}

size_t Level::getCurrentSize() {
    return dataBlocks.size();
}

void Level::addTuple(Tuple *tuple) {
    if (isFull()) {
        throw LevelFullException();
    }
    if (isEmpty()) {
        createAndInsertNewFileMeta();
        dataBlocks[0]->appendTupleToFile(tuple);
    } else {
        if (dataBlocks[dataBlocks.size() - 1]->isFull()) {
            createAndInsertNewFileMeta();
        }
        dataBlocks[dataBlocks.size() - 1]->appendTupleToFile(tuple);
    }
}

FileMeta *Level::merge(size_t newLvlID, size_t newBlockIdx) {
    Run* initRun = new Run(MAX_RUN_NUM * MAX_TUPLE_NUM_IN_RUN);
    for (FileMeta* fm : dataBlocks) {
        initRun->merge(fm->getRun());
    }
    clear();
    return initRun->createFileMetaFromRun(newLvlID, newBlockIdx);
}

void Level::clear() {
    while (!dataBlocks.empty()) {
        FileMeta* fmToDel = dataBlocks.back();
        dataBlocks.pop_back();
        delete fmToDel;
    }
}

bool Level::isEmpty() {
    return dataBlocks.empty();
}

FileMeta *Level::getDataMeta(int idx) {
    return dataBlocks[idx];
}

void Level::addRunFileMeta(FileMeta *fm) {
    if (!isFull()) {
        dataBlocks.push_back(fm);
    } else {
        throw LevelFullException();
    }
}

Run *Level::getRunByFileMetaAtIndex(int idx) {
    return getDataMeta(idx)->getRun();
}

void Level::createAndInsertNewFileMeta() {
    stringstream ss;
    ss << "./";
    ss << "level-";
    ss << lvlID;
    ss << "-block-";
    ss << dataBlocks.size();
    ss << ".txt";
    string newFilePath = ss.str();
    ofstream newFile(newFilePath);
    newFile.close();
    addRunFileMeta(new FileMeta(newFilePath, MAX_TUPLE_NUM_IN_RUN));
}
