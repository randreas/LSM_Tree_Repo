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
    //TODO: change this
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