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
    return tuples.size() == MAX_TUPLE_NUM;
}

void Run::clear() {
    while (!tuples.empty()) {
        Tuple* tuple = tuples.back();
        tuples.pop_back();
        delete tuple;
        tuple = nullptr;
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

void Run::printRun() {
    cout << "Run: \n";
    cout << "tuple count: " << tuples.size() << "\n";
    for (Tuple* tuple: tuples) {
        tuple->printTuple();
    } 
    cout << "done\n";
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

vector<Tuple *> Run::getTuples() {
    return tuples;
}
