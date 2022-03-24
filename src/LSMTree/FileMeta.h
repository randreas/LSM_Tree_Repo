//
// Created by jingyusu on 3/20/22.
//

#ifndef TEMPLATEDB_FILEMETA_H
#define TEMPLATEDB_FILEMETA_H

#include <string>
#include <utility>
#include "run.h"

using namespace std;

class FileMeta {
private:
    int size;
    int minKey, maxKey;

public:
    const size_t MAX_TUPLE_NUM;

    explicit FileMeta(string filePath, size_t max_tuple_num) : MAX_TUPLE_NUM(max_tuple_num) {
        this->filePath = std::move(filePath);
        this->size = 0;
        this->minKey = INT32_MAX;
        this->maxKey = INT32_MIN;
    }

    string getFilePath() {
        return this->filePath;
    }

    //Initiate run object
    Run* getRun();

    //Datablock is full
    bool isFull() const;

    //Append tuple to the end of file
    void appendTupleToFile(Tuple* tuple);

    string filePath;
};

class RunFullException : public exception {

};


#endif //TEMPLATEDB_FILEMETA_H
