//
// Created by jingyusu on 3/16/22.
//

#ifndef TEMPLATEDB_RUN_H
#define TEMPLATEDB_RUN_H

#include "../tuple.h"

class FileMeta;

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
    explicit Run(size_t size);

    //If this run is full
    bool isFull();

    //Merge this run with input run
    void merge(Run* anotherRun);

    //Create a filemeta corresponding to this run. Use this only when merge
    FileMeta* createFileMetaFromRun(size_t lvlID, size_t newBlockIdx);


    //add tuple to the run, used only in instantiation.
    void addTuple(Tuple* newTuple);

    ~Run();
};


#endif //TEMPLATEDB_RUN_H
