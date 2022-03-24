//
// Created by jingyusu on 3/16/22.
//

#ifndef TEMPLATEDB_RUN_H
#define TEMPLATEDB_RUN_H

#include "tuple.h"

class Run {

private:
    //tuples in this run
    vector<Tuple*> tuples;

    //Replaces current tuples vector with a provided vector
    void replaceTuplesWithInput(vector<Tuple*> newTuples);

public:
    //Maximum size of this run
    const size_t MAX_TUPLE_NUM;

    //Constructor
    Run(size_t size);

    //If this run is full
    bool isFull();

    //Deep clear all tuple objects
    void clear();

    //Simple empty tuples vector
    void shallowClear();

    //Merge this run with input run, input run(another run) should always be more update than the current run.
    void merge(Run* anotherRun);

    //add tuple to the run, used only in instantiation.
    void addTuple(Tuple* newTuple);

    bool containsKey(int key);

    Tuple* query(int key);

    void printRun();

    vector<Tuple*> getTuples();

    ~Run();
};

class RunFException : public exception {

};


#endif //TEMPLATEDB_RUN_H
