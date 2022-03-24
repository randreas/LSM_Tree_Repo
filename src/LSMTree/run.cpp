//
// Created by jingyusu on 3/16/22.
//

#include "run.h"
#include "FileMeta/FileMeta.h"

#include <utility>
#include <sstream>
#include <fstream>
#include <iostream>
#include "itoa.h"

Run::Run(size_t size) : MAX_TUPLE_NUM(size) {
    tuples = vector<Tuple*>(size);
}

bool Run::isFull() {
    return tuples.size() == MAX_TUPLE_NUM;
}

void Run::clear() {
    while (!tuples.empty()) {
        Tuple* tuple = tuples.back();
        tuples.pop_back();
        delete tuple;
    }
}

void Run::merge(Run *anotherRun) {
    vector<Tuple*> newTuples;
    while (!(anotherRun->tuples.empty() && this->tuples.empty())) {
        if (this->tuples.empty() || anotherRun->tuples[0]->key < this->tuples[0]->key) {
            newTuples.push_back(anotherRun->tuples[0]);
            anotherRun->tuples.erase(anotherRun->tuples.begin());
        } else if (anotherRun->tuples.empty() || this->tuples[0]->key < anotherRun->tuples[0]->key) {
            newTuples.push_back(this->tuples[0]);
            this->tuples.erase(this->tuples.begin());
        } else if (this->tuples[0]->key == anotherRun->tuples[0]->key) {
            //deletion
            if (anotherRun->tuples[0]->isDeleteMarker()) {
                delete this->tuples[0];
                this->tuples.erase(this->tuples.begin());
                delete anotherRun->tuples[0];
                anotherRun->tuples.erase(this->tuples.begin());
            } else { //update
                delete this->tuples[0];
                this->tuples.erase(this->tuples.begin());
                newTuples.push_back(anotherRun->tuples[0]);
                anotherRun->tuples.erase(anotherRun->tuples.begin());
            }
        }
    }
    replaceTuplesWithInput(newTuples);
}

void Run::addTuple(Tuple *newTuple) {
    if (!isFull()) {
        auto it = tuples.begin();
        int pos = 0;
        while (it != tuples.end() && (*it)->key <= newTuple->key) {
            if ((*it)->key == newTuple->key) {
                tuples.erase(tuples.begin()+pos);
                break;
            }
            it++;
            pos++;
        }
        tuples.insert(it, newTuple);
    } else {
        throw RunFullException();
    }
}

bool Run::containsKey(int key) {
    for (Tuple* tuple : tuples) {
        if (tuple->key == key) {
            return true;
        }
    }
    return false;
}

Tuple* Run::query(int key) {
    for (Tuple* tuple : tuples) {
        if (tuple->key == key) {
            return tuple;
        }
    }
    throw RunFException();
}


FileMeta *Run::createFileMetaFromRun(size_t lvlID, size_t newBlockIdx) {
    //TODO: Replace with binary file
    stringstream ss;
    ss << "level-";
    ss << lvlID;
    ss << "-block-";
    ss << newBlockIdx;
    ss << ".txt";
    string newFilePath = ss.str();
    char* path = const_cast<char*>(newFilePath.c_str());

    if (FILE *file = fopen(path, "r")) {
        fclose(file);
        remove(path);
    }

    ofstream newFile(newFilePath);
    for (Tuple* tuple : tuples) {
        string str = itoa(tuple->key) + " ";
        for (int val : tuple->value.items) {
            str += itoa(tuple->key) + " ";
        }
        str += "\n";
        newFile << str;
    }
    auto* newFileMeta = new FileMeta(newFilePath, MAX_TUPLE_NUM);
    newFile.close();
    return newFileMeta;
}

Run::printRun() {
    count << "Run: \n";
    for (Tuple* tuple: tuples) {
        tuple->printTuple();
    } 
}

Run::~Run() {
    clear();
}

void Run::replaceTuplesWithInput(vector<Tuple *> newTuples) {
    this->tuples = std::move(newTuples);
}

void Run::shallowClear() {
    tuples.clear();
}
