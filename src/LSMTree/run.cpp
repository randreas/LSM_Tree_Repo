//
// Created by jingyusu on 3/16/22.
//

#include "run.h"
#include "FileMeta.h"

#include <utility>
#include <sstream>
#include <fstream>
#include <iostream>
#include "itoa.h"

Run::Run(size_t size) : MAX_TUPLE_NUM(size) {
}

bool Run::isFull() {
    return tuples.size() >= MAX_TUPLE_NUM;
}

void Run::clear() {
    /*
    while (!tuples.empty()) {
        LSMTuple::Tuple* tuple = tuples.back();
        tuples.pop_back();
        delete tuple;
        tuple = nullptr;
    }
     */
    tuples.clear();
}

//anotherRun is more up-to-date
void Run::merge(Run *anotherRun) {
//    cout << "In run.merge\n";
//    cout << "This run: \n";
//    printRun();
//    cout << "Run to merge: \n";
//    anotherRun->printRun();
    vector<LSMTuple::Tuple*> newTuples;
    while (!anotherRun->tuples.empty() && !this->tuples.empty()) {
        if (this->tuples.empty() || anotherRun->tuples[0]->key < this->tuples[0]->key) {
            newTuples.push_back(anotherRun->tuples[0]);
            anotherRun->tuples.erase(anotherRun->tuples.begin());
        } else if (anotherRun->tuples.empty() || this->tuples[0]->key < anotherRun->tuples[0]->key) {
            newTuples.push_back(this->tuples[0]);
            this->tuples.erase(this->tuples.begin());
        } else if (this->tuples[0]->key == anotherRun->tuples[0]->key) {
            delete this->tuples[0];
            this->tuples.erase(this->tuples.begin());
            newTuples.push_back(anotherRun->tuples[0]);
            anotherRun->tuples.erase(anotherRun->tuples.begin());
        }
    }
    if (!anotherRun->tuples.empty()) {
        for (LSMTuple::Tuple* tuple : anotherRun->tuples) {
            newTuples.push_back(tuple);
        }
    } else if (!this->tuples.empty()) {
        for (LSMTuple::Tuple* tuple : this->tuples) {
            newTuples.push_back(tuple);
        }
    }
    // cout << "New tuple vector: \n";
    // for (LSMTuple::Tuple* t : newTuples) {
    //     t->printTuple();
    // }
    // cout << "Merge done\n";
    replaceTuplesWithInput(newTuples);
    // cout << "Replace done\n";
}

void Run::replaceTuplesWithInput(const vector<LSMTuple::Tuple *>& newTuples) {
    //this->tuples = std::move(newTuples);
    //cout << "In replace: \n";
    /*
    for (Tuple* tuple : this->tuples) {
        delete tuple;
    }
     */
    clear();
    //cout << "Cleared\n";
    for (LSMTuple::Tuple* tuple : newTuples) {
        this->tuples.push_back(tuple);
    }
}

void Run::addTuple(LSMTuple::Tuple *newTuple) {
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
    for (LSMTuple::Tuple* tuple : tuples) {
        if (tuple->key == key) {
            return true;
        }
    }
    return false;
}

LSMTuple::Tuple* Run::query(int key) {
    for (LSMTuple::Tuple* tuple : tuples) {
        if (tuple->key == key) {
            return tuple;
        }
    }
    throw RunFException();
}

void Run::printRun() {
    // cout << "--------------\n";
    // cout << "| Run: \n";
    // cout << "| tuple count: " << tuples.size() << "\n";
    // for (LSMTuple::Tuple* tuple: tuples) {
    //     tuple->printTuple();
    // }
    // cout << "| done\n";
    // cout << "--------------\n";
}

Run::~Run() {
    clear();
}

void Run::shallowClear() {
    tuples.clear();
}

vector<LSMTuple::Tuple *> Run::getTuples() {
    return tuples;
}

int Run::getSize() {
    return tuples.size();
}
