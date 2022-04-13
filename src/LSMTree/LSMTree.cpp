#include <stdlib.h>
#include <string.h>
#include <unordered_set>

#include "LSMTree.h"
#include "run.h"
#include "Utils.h"

using namespace std;

LSMTree::LSMTree(int _initial_run_size, int _num_run_per_level) {
    initial_run_size = _initial_run_size;
    num_run_per_level = _num_run_per_level;
    buffer = new Run(initial_run_size);
}

void LSMTree::addTuple(Tuple* tuple) {
    // check if buffer is full
    // full, move run to level 1, clear buffer
    cout << "in add tuple\n";
    if (buffer->isFull()) {
        //FileMeta* bufferFile = buffer->createFileMetaFromRun(0, 0);  // level 0, index 0
        cout << "Buffer is full, need to flush\n";
        Run* push_run = new Run(buffer->MAX_TUPLE_NUM + 1);
        push_run->merge(buffer);
        push_run->addTuple(tuple);
        cout << "run merged\n";
        push_run->printRun();
        mergeNMove(0, push_run);
        //cout << "finished merge and move\n";
        //remove(const_cast<char*>(bufferFile->filePath.c_str()));
        buffer->shallowClear();
    } else {
        buffer->addTuple(tuple);
        cout << "Buffer after insert: \n";
        buffer->printRun();
    }
}

Tuple* LSMTree::query(int key) {
    if (buffer->containsKey(key)) {
        return buffer->query(key);
    }
    cout << "query 0\n";
    int ind;
    for (Level *curLevel: levels) {
        cout << "query 1\n";

        //Check bloomFilter
        ind = curLevel->containsKey(key);
        cout << "query 2\n";
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
    cout << "in move to level\n";
    if (idx == levels.size()) {
        cout << "here1\n";
        int newRunSize = idx == 0 ? newRun->MAX_TUPLE_NUM : (levels[levels.size() - 1]->MAX_TUPLE_NUM_IN_RUN + 1) * num_run_per_level;
        //cout << newRunSize << "\n";
        int lvlId = levels.size();
        //cout << lvlId << "\n";
        levels.push_back(new Level(num_run_per_level, newRunSize, lvlId));
        //cout << levels.size() << "\n";
        levels[lvlId]->addRunFileMeta(createFileMetaFromRun(lvlId, 0, newRun));
        //delete newRun;
        cout << "level of size: " << levels.size() << "\n";
    } else {
        cout << "here2\n";
        Level *lvl = levels[idx];
        if (!lvl->isFull()) {
            lvl->addRunFileMeta(createFileMetaFromRun(idx, lvl->getCurrentSize(), newRun));
            //delete newRun;
        } else {
            cout << "level " << lvl->lvlID << " is full\n";
            Run* mergedResult = lvl->merge();
            cout << "level merged\n";
            cout << "level " << lvl->lvlID << " merged result:\n";
            mergedResult->printRun();
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



vector<Tuple*> LSMTree::query(int low, int high) {
    vector<Tuple*> result;
    unordered_set <int> set;
    
    for(Tuple* t : buffer->getTuples()) {
        int key = t-> key;
        if(key <= high && key >= low) {
            if(set.find(key) != set.end()) {
                cout << "key already is in the set";
            } else {
                cout << "new key found adding into result";
                result.push_back(t);
                set.insert(t->key);
            }
        }
    }

    for (Level *curLevel: levels) {
        // Todo: check the fence pointer if low and high are in the range
        // Get the run using index if it matches
        vector<int> zoneIdxs = curLevel->fp->query(low, high);
        for(int idx : zoneIdxs) {
            Run* currRun = curLevel->getRunByFileMetaAtIndex(idx);
            vector<Tuple*> curr_tuples = currRun->getTuples();
            for(Tuple* t :curr_tuples) {
                int key = t->key;
                if(key <= high && key >= low) {
                    if(set.find(key) != set.end()) {
                        cout << "key already is in the set";
                    } else {
                        cout << "new key found adding into result";
                        result.push_back(t);
                        set.insert(key);
                    }
                }
            }
        }
        
    }
    return result;

}