#ifndef LSMTREE_H
#define LSMTREE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <cmath>

#include "run.h"
#include "level.h"
#include "tuple.h"

class LSMTree
{
    int initial_run_size, num_run_per_level;
    vector<Level*> levels;

public:
    Run* buffer;

    LSMTree(int _initial_run_size, int _num_run_per_level);

    /**
     * Return all index of file meta that contains the key, null if the key does not exist in this fence pointer
     */
    Tuple* query(int key);

    /**
     * Given a high and a low, return all index of file meta that contains key in the range
     * @param low
     * @param high
     * @return
     */
    Tuple* query(int low, int high);

    void addTuple(Tuple* tuple);

    void deleteKey(int key);


private:
    void mergeNMove(int sourceLevel, Run* newRun);

    void moveToLevelAtIdxRecurse(int idx, Run *newRun);
};

class FencePointerException : public exception {};

#endif