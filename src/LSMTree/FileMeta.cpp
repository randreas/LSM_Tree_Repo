//
// Created by jingyusu on 3/20/22.
//

#include "FileMeta.h"
#include <iostream>
#include "tuple.h"
#include "run.h"
#include "Utils.h"

Run *FileMeta::getRun() {
    cout << "in getRun of file " << filePath << "\n";
    ifstream inFile(filePath, ios::in|ios::binary);
    Run* run = new Run(MAX_TUPLE_NUM);
    //Read # of tuple from file
    readRunFromData(&inFile, run);
    return run;
}


bool FileMeta::isFull() const {
    return MAX_TUPLE_NUM == size;
}

/*Deprecated
void FileMeta::appendTupleToFile(Tuple *tuple) {
    if (isFull()) {
        throw RunFullException();
    } else {
        ofstream file(filePath, ios::out|ios::binary);
        file << tuple->key;
        //string str = itoa(tuple->key) + " ";
        for (int val : tuple->value.items) {
            //str += itoa(tuple->key) + " ";
            file << val;
        }
        //str += "\n";
        //file << str;
        size++;
        if (tuple->key > maxKey) {
            maxKey = tuple->key;
        }
        if (tuple->key < minKey) {
            minKey = tuple->key;
        }
    }
}
 */

FileMeta::~FileMeta() {
    if (FILE *file = fopen(const_cast<char*>(filePath.c_str()), "r")) {
        fclose(file);
        remove(filePath.c_str());
    }
}

//RA Todo
vector<LSMTuple::Tuple*> FileMeta::GetAllTuples() {
    Run* run =  getRun();
    return run->getTuples();
}