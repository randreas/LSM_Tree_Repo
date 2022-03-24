#include <stdlib.h>
#include <string.h>

#include "LSMTree.h"

using namespace std;

LSMTree::LSMTree(int _initial_run_size, int _num_run_per_level) {
    initial_run_size = _initial_run_size;
    num_run_per_level = _num_run_per_level;
    buffer = new Run(initial_run_size);
}

void LSMTree::addTuple(Tuple* tuple) {
    // check if buffer is full
    // full, move run to level 1, clear buffer
    if (buffer->isFull()) {
        //FileMeta* bufferFile = buffer->createFileMetaFromRun(0, 0);  // level 0, index 0
        Run* push_run = new Run(buffer->MAX_TUPLE_NUM);
        push_run->merge(buffer);
        mergeNMove(0, push_run);
        //remove(const_cast<char*>(bufferFile->filePath.c_str()));
        buffer->shallowClear();
    }
}

Tuple* LSMTree::query(int key) {
    if (buffer->containsKey(key)) {
        return buffer->query(key);
    }

    int ind;
    for (Level *curLevel: levels) {
        ind = curLevel->containsKey(key);
        if (ind >= 0) {
            return curLevel->getRunByFileMetaAtIndex(ind)->query(key);
        }
    }

    // if not found, return a tuple with delete flag
    return new Tuple(key, Value(false));
}

void LSMTree::mergeNMove(int idx, Run* newRun) {
    moveToLevelAtIdxRecurse(idx, newRun);
}

void LSMTree::moveToLevelAtIdxRecurse(int idx, Run* newRun) {
    if (idx == levels.size()) {
        int newRunSize = idx == 0 ? initial_run_size : (levels[levels.size() - 1]->MAX_TUPLE_NUM_IN_RUN + 1) * num_run_per_level;
        int lvlId = levels.size();
        levels.push_back(new Level(num_run_per_level, newRunSize, lvlId));
        levels[lvlId]->addRunFileMeta(newRun->createFileMetaFromRun(lvlId, 0));
        //delete newRun;
    } else {
        Level *lvl = levels[idx];
        if (!lvl->isFull()) {
            lvl->addRunFileMeta(newRun->createFileMetaFromRun(idx, lvl->getCurrentSize()));
            //delete newRun;
        } else {
            Run* mergedResult = lvl->merge();
            mergedResult->merge(newRun);
            moveToLevelAtIdxRecurse(idx + 1, mergedResult);
        }
    }
    delete newRun;
    newRun = nullptr;
}

void LSMTree::deleteKey(int key) {
    addTuple(new Tuple(key, Value(false)));
}
