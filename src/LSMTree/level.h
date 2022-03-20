//
// Created by jingyusu on 3/16/22.
//

#ifndef TEMPLATEDB_LEVEL_H
#define TEMPLATEDB_LEVEL_H

#include "Run/run.h"
#include <vector>

class Level {

private:
    std::vector<Run*> runs;

    //Clear all the runs in the level
    void clear();

public:
    //Maximum number of runs per level
    const size_t MAX_RUN_NUM;

    const size_t MAX_TUPLE_NUM_IN_RUN;

    explicit Level(int max_run_num, int max_tuple_num_in_run) : MAX_RUN_NUM(max_run_num), MAX_TUPLE_NUM_IN_RUN(max_tuple_num_in_run) {
        runs = std::vector<Run*>(MAX_RUN_NUM);
    }

    //If this level is full
    bool isFull();

    //If this level is empty
    bool isEmpty();

    //Get the run at idx
    Run* getRun(int idx);

    //Get current size
    size_t getCurrentSize();

    //Add a run to the level. Happens when the upper level merges.
    void addRun(Run* run);

    //Add a tuple
    void addTuple(Tuple* tuple);

    //Merges the entire level, return pointer of a merged run
    Run* merge();

};

class LevelFullException : public exception {

};


#endif //TEMPLATEDB_LEVEL_H
