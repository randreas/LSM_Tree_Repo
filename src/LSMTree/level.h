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

public:
    //Maximum number of runs per level
    const int MAX_RUN_NUM;

    explicit Level(int max_run_num) : MAX_RUN_NUM(max_run_num) {
        runs = std::vector<Run*>(MAX_RUN_NUM);
    }

    //If this level is full
    bool isFull();

    //Get the run at idx
    Run* getRun(int idx);

    //Get current size
    size_t getCurrentSize();

    //Add a run to the level. Happens when the upper level merges.
    void addRun(Run* run);

    //Add a tuple
    void addTuple(Tuple* tuple);

    //Merges this level and a given level. Returns merged result, which is a new object
    Level* merge(Level* anotherLevel);

};

class LevelFullException : public exception {

};


#endif //TEMPLATEDB_LEVEL_H
