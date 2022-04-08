//
// Created by jingyusu on 3/20/22.
//

#include "FileMeta.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "itoa.h"
#include "tuple.h"
#include "run.h"

Run *FileMeta::getRun() {
    ifstream *inFile = nullptr;
    inFile->open(filePath, ios::in|ios::binary);
    Run* run = new Run(MAX_TUPLE_NUM);
    //Read # of tuple from file
    int tupleCnt;
    inFile->read(reinterpret_cast<char *>(&tupleCnt), sizeof tupleCnt);
    for (int i = 0; i < tupleCnt; i++) {
        int offset = sizeof(int) * (i + 1);
        int nextTupleOffset = sizeof(int) * (i + 2);
        inFile->seekg(offset);
        //Read key from file
        int key;
        inFile->read(reinterpret_cast<char *>(&key), sizeof(int));
        offset += sizeof(int);
        //Read val items from file
        Value val;
        while (offset < nextTupleOffset) {
            int valItem;
            inFile->seekg(offset);
            inFile->read(reinterpret_cast<char *>(&valItem), sizeof(int));
            val.items.push_back(valItem);
            offset += sizeof(int);
        }
        auto* newTuple = new Tuple(key, val);
        run->addTuple(newTuple);
    }
    inFile->close();
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
vector<Tuple*> FileMeta::GetAllTuples() {
    Run* run =  getRun();
    return run->getTuples();
}