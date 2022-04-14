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

const int SIZE_OF_INT = sizeof(int);

Run *FileMeta::getRun() {
    cout << "in getRun of file " << filePath << "\n";
    ifstream *inFile = new ifstream (filePath, ios::in|ios::binary);
    Run* run = new Run(MAX_TUPLE_NUM);
    //Read # of tuple from file
    int tupleCnt;
    inFile->read(reinterpret_cast<char *>(&tupleCnt), SIZE_OF_INT);
    for (int i = 0; i < tupleCnt; i++) {
        //cout << "tuple " << i << "\n";
        inFile->seekg(sizeof(int) * (i + 1));
        int offset;
        inFile->read(reinterpret_cast<char *>(&offset), SIZE_OF_INT);
        inFile->seekg(sizeof(int) * (i + 2));
        int nextTupleOffset;
        inFile->read(reinterpret_cast<char *>(&nextTupleOffset), SIZE_OF_INT);
        //cout << "offset: " << offset << ", next offset: " << nextTupleOffset << "\n";
        inFile->seekg(offset);
        //Read key from file
        int key;
        inFile->read(reinterpret_cast<char *>(&key), SIZE_OF_INT);
        offset += SIZE_OF_INT;
        //Read val items from file
        Value val;
        while (offset < nextTupleOffset) {
            int valItem;
            inFile->seekg(offset);
            inFile->read(reinterpret_cast<char *>(&valItem), SIZE_OF_INT);
            val.items.push_back(valItem);
            offset += SIZE_OF_INT;
        }
        auto* newTuple = new Tuple(key, val);
        run->addTuple(newTuple);
    }
    //cout << "generated run: \n";
    //run->printRun();
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