#ifndef LSMTREE_H
#define LSMTREE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <cmath>

#include "run.h"
#include "level.h"

class LSMTree
{
    int initial_run_size, num_run_per_level;
    Run buffer;
    vector<Level> levels;

public:

    LSMTree(int _initial_run_size, int _num_run_per_level);

    /**
     * Return all index of file meta that contains the key, null if the key does not exist in this fence pointer
     */
    std::vector<int> query(int key);

    /**
     * Given a high and a low, return all index of file meta that contains key in the range
     * @param low
     * @param high
     * @return
     */
    std::vector<int> query(int low, int high);

    void addTuple(Tuple* tuple);

    void delete(int key);

    void mergeNMove(int sourceLevel);
};

class FencePointerException : public exception {};

#endif