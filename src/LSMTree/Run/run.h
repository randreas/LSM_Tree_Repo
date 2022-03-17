//
// Created by jingyusu on 3/16/22.
//

#ifndef TEMPLATEDB_RUN_H
#define TEMPLATEDB_RUN_H

#include "../tuple.h"

class Run {

private:
    //tuples in this run
    vector<Tuple*> tuples;

public:
    //Maximum size of this run
    const int MAX_TUPLE_NUM;

    //Constructor
    explicit Run(int size);

    //If this run is full
    bool isFull();

};


#endif //TEMPLATEDB_RUN_H
