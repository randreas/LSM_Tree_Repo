
//
// Created by xingk on 3/22/2022.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>

#include "./LSMTree.h"
#include "./tuple.h"

class KeyException : public exception {};

using namespace std;

void printIntVector(vector<int> v) {
    for (int e : v) {
        cout << e << " ";
    }
}

void executeCommand(LSMTree* lsmTree, string command) {
    stringstream iss(command);
    vector<string> elements;
    string e;
    
    // read all elements into a vector of strings
    int counter = 0;
    while(getline(iss, e, ' '))
    {
        elements.push_back(e);
    }

    // start execute the command
    if (elements.size() < 2) {
        return;
    }

    if (elements[0] == "I") {
        int key = stoi(elements[1]);
        vector<int> values;
        for (string cur_e : vector<string>(elements.begin() + 2, elements.end())) {
            values.push_back(stoi(cur_e));
        }
        cout << "---------------------------------------\n";
        cout << "Insert " << "key: " << key << " values: ";
        printIntVector(values);
        cout << "\n";
        // execute
        lsmTree->addTuple(new LSMTuple::Tuple(key, LSMTuple::Value(values)));
        cout << "Finished adding LSMTree.addTuple with key " << key << "\n";
        //lsmTree->buffer->printRun();
        //if (lsmTree->buffer == nullptr) {
        //    cout << "buffer is null\n";
        //}
    } else if (elements[0] == "Q") {
        if (elements.size() != 2) {
            cout << "Q with incorrect size\n";
            return;
        }
        int key = stoi(elements[1]);
        cout << "---------------------------------------\n";
        cout << "Point query key: " << key << "\n";
        // execute
        LSMTuple::Tuple* resultTuple = lsmTree->query(key);
        //cout << "gate 0\n";
        if (resultTuple->key != key) {
            throw KeyException();
        }
        //cout << "gate 1\n";
        if (resultTuple->isDeleteMarker()) {
            cout << "query result : key: " << key << " not in the lsm tree, not entered or deleted" << "\n";
        } else {
            cout << "query result : key: " << key << " value: ";
            resultTuple->getValue().printValue();
            cout<< "\n";
        }
    } else if (elements[0] == "S") {
        if (elements.size() != 3) {
            cout << "S with incorrect size\n";
            return;
        }
        int key_low = stoi(elements[1]);
        int key_high = stoi(elements[2]);
        cout << "Range query " << "low key: " << key_low << " high key: " << key_high << "\n";
        vector<LSMTuple::Tuple*> resultTuples = lsmTree->query(key_low, key_high);
        cout << "resultTuples size = " << resultTuples.size() << "\n";
        if(resultTuples.size() > 0) {
            for (LSMTuple::Tuple* t : resultTuples) {
                t->getValue().printValue();
            }
        }



    } else if (elements[0] == "D") {
        // if (elements.size() > 3) {
        //     cout << "D with incorrect size\n";
        //     return;
        // }
        cout << "D  size = " << elements.size() << "\n";
        if (elements.size() == 2) {
            int key = stoi(elements[1]);
            cout << "---------------------------------------\n";
            cout << "Delete " << "key: " << key << "\n";
            // execute
            lsmTree->deleteKey(key);
           
            cout << "delete; addtuple finished\n";
        } else if (elements.size() == 3) {
            int low = stoi(elements[1]);
            int high = stoi(elements[2]);
            cout << "---------------------------------------\n";
           
            // execute
            lsmTree->deleteKey(low,high);
            cout << "delete; addtuple finished\n";
        } else {
            cout << "D with incorrect size\n";
            return;
        }
    }
}

void executeQueryFile(LSMTree* lsmTree, string filePath) {
    ifstream fs;
    fs.open(filePath);

    string line;
    if (fs.is_open()) {
        // cout << "In executeQueryFile(), file string is open.\n";
        while (getline(fs, line)) {
            // fs >> line;
            executeCommand(lsmTree, line);
        }
        // while (fs.good()) {
        //     fs >> line;
        //     cout << line << "\n";
        // }
    } else {
        cout << "In executeQueryFile(), file string is close.\n";
    }
}

int main(int argc, char *argv[])
{
    if (argc !=  2) {
        cout << "USAGE: ./main <data file path> \n";
        return 1;
    }

    // constant, hyper parameter
    int initial_run_size = 3;
    int num_run_per_level = 3;
    bool isTiering = false;

    // constant, hyper parameter
    char* inputFilePath = argv[1];

    // create levels for this tree
    LSMTree* lsmTree = new LSMTree(initial_run_size, num_run_per_level, isTiering);
    lsmTree->open();
    cout << "LSMTREE OPEN finished\n";
    // read and execute data file
    cout << "buffer: \n";
    lsmTree->buffer->printRun();
    cout << "tree level number: " << lsmTree->getLevelCnt() << "\n";
    cout << "Start reading and executing data file\n";
    executeQueryFile(lsmTree, inputFilePath);
    lsmTree->close();
     
    return 0;
}
