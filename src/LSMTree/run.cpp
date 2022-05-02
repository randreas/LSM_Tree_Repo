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
    vector<LSMTuple::Tuple*> newTuples;
    // going through all tuples in both run, stop when one of them is empty
    while (!anotherRun->tuples.empty() && !this->tuples.empty()) {
        if (this->tuples.empty() || anotherRun->tuples[0]->key < this->tuples[0]->key) {
            newTuples.push_back(anotherRun->tuples[0]);
            anotherRun->tuples.erase(anotherRun->tuples.begin());
        } else if (anotherRun->tuples.empty() || this->tuples[0]->key < anotherRun->tuples[0]->key) {
            newTuples.push_back(this->tuples[0]);
            this->tuples.erase(this->tuples.begin());
        } else if (this->tuples[0]->key == anotherRun->tuples[0]->key) {
            // is same key, delete one value
            delete this->tuples[0];
            this->tuples.erase(this->tuples.begin());
            newTuples.push_back(anotherRun->tuples[0]);
            anotherRun->tuples.erase(anotherRun->tuples.begin());
        }
    }
    // when one run is empty while the other run isn't, push all the tuples of the not empty run into the new run
    if (!anotherRun->tuples.empty()) {
        for (LSMTuple::Tuple* tuple : anotherRun->tuples) {
            newTuples.push_back(tuple);
        }
    } else if (!this->tuples.empty()) {
        for (LSMTuple::Tuple* tuple : this->tuples) {
            newTuples.push_back(tuple);
        }
    }
    replaceTuplesWithInput(newTuples);
}

void Run::replaceTuplesWithInput(const vector<LSMTuple::Tuple *>& newTuples) {
    clear();
    for (LSMTuple::Tuple* tuple : newTuples) {
        this->tuples.push_back(tuple);
    }
}

// add a tuple to the run
void Run::addTuple(LSMTuple::Tuple *newTuple) {
    if (!isFull()) {
        auto it = tuples.begin();
        int pos = 0;
        // check if the run already contains the key, if so, replace it
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

// going through tuples to see if there is a match in the key
bool Run::containsKey(int key) {
    for (LSMTuple::Tuple* tuple : tuples) {
        if (tuple->key == key) {
            return true;
        }
    }
    return false;
}

LSMTuple::Tuple* Run::binarySearch(int key, int l, int r) {
    if (r >= l) {
        int mid = l + (r - l) / 2;

        if (tuples[mid]->key == key)
            return tuples[mid];

        if (tuples[mid]->key > key)
            return binarySearch(key, l, mid - 1);

        return binarySearch(key, mid + 1, r);
    }

    throw RunFException();
}

// return the tuple that the key match
LSMTuple::Tuple* Run::query(int key) {
    return binarySearch(key, 0, tuples.size() - 1);
//    for (LSMTuple::Tuple* tuple : tuples) {
//        if (tuple->key == key) {
//            return tuple;
//        }
//    }
//    throw RunFException();
}

// print the status of the run
void Run::printRun() {
    cout << "--------------\n";
    cout << "| Run: \n";
    cout << "| tuple count: " << tuples.size() << "\n";
    for (LSMTuple::Tuple* tuple: tuples) {
        tuple->printTuple();
    }
    cout << "| done\n";
    cout << "--------------\n";
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
