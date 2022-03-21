//
// Created by jingyusu on 3/16/22.
//

#ifndef TEMPLATEDB_RUN_H
#define TEMPLATEDB_RUN_H

#include "../tuple.h"
#include "LSMTree/FileMeta/FileMeta.h"

class Run {

private:
    //tuples in this run
    vector<Tuple*> tuples;

public:
    //Maximum size of this run
    const size_t MAX_TUPLE_NUM;

    //Constructor
    explicit Run(size_t size);

    //If this run is full
    bool isFull();

    Run* merge(Run* anotherRun);

    void addTuple(Tuple* newTuple);

    //Create a filemeta corresponding to this run
    FileMeta* createFileMetaFromRun();

    ~Run();
};


#endif //TEMPLATEDB_RUN_H
