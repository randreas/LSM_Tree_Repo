//
// Created by jingyusu on 3/20/22.
//

#include "FileMeta.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../itoa.h"

Run *FileMeta::getRun() {
    ifstream inFile;
    inFile.open(filePath, ios::in);
    Run* run = new Run(MAX_TUPLE_NUM);
    string s;
    while ( getline(inFile,s) ) {
        stringstream strStream(s);
        string temp;
        int key;
        Value val;
        bool readKey = false;
        while (getline(strStream, temp, ' ')) {
            if (temp.compare("\n") == 0) {
                break;
            }
            if (!readKey) {
                readKey = true;
                key = stoi(temp);
            } else {
                val.items.push_back(stoi(temp));
            }
        }
        auto* newTuple = new Tuple(key, val);
        run->addTuple(newTuple);
    }
    inFile.close();
    return run;
}

bool FileMeta::isFull() const {
    return MAX_TUPLE_NUM == size;
}

void FileMeta::appendTupleToFile(Tuple *tuple) {
    if (isFull()) {
        throw RunFullException();
    } else {
        ofstream file(filePath);
        string str = itoa(tuple->key) + " ";
        for (int val : tuple->value.items) {
            str += itoa(tuple->key) + " ";
        }
        str += "\n";
        file << str;
        size++;
        if (tuple->key > maxKey) {
            maxKey = tuple->key;
        }
        if (tuple->key < minKey) {
            minKey = tuple->key;
        }
    }
}
