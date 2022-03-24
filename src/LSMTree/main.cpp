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
    
    int key;
    key = stoi(elements[1]);
    if (elements[0] == "I") {
        vector<int> values;
        for (string cur_e : vector<string>(elements.begin() + 2, elements.end())) {
            values.push_back(stoi(cur_e));
        }
        cout << "Insert " << "key: " << key << " values: ";
        printIntVector(values);
        cout << "\n";
        // execute
        lsmTree->addTuple(new Tuple(key, Value(values)));
    } else if (elements[0] == "Q") {
        if (elements.size() != 2) {
            cout << "Q with incorrect size\n";
            return;
        }
        cout << "Point query " << "key: " << key << "\n";
        // TODO execute
    } else if (elements[0] == "S") {
        if (elements.size() != 3) {
            cout << "D with incorrect size\n";
            return;
        }
        int key_low = stoi(elements[1]);
        int key_high = stoi(elements[2]);
        cout << "Range query " << "low key: " << key_low << " high key: " << key_high << "\n";
        // TODO execute
    } else if (elements[0] == "D") {
        if (elements.size() != 2) {
            cout << "D with incorrect size\n";
            return;
        }
        cout << "Delete " << "key: " << key << "\n";
        // TODO execute
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
    if (argc !=  5) {
        cout << "USAGE: ./main <initial run size> <number of runs per level> <data file path> <workload file path>\n";
        return 1;
    }

    // constant, hyper parameter
    int initial_run_size = stoi(argv[1]);
    int num_run_per_level = stoi(argv[2]);
    char* dataFilePath = argv[3];
    char* workloadFilePath = argv[4];

    // create levels for this tree
    LSMTree* lsmTree = new LSMTree(initial_run_size, num_run_per_level);

    // read and execute data file
    cout << "Start reading and executing data file\n";
    executeQueryFile(lsmTree, dataFilePath);

    // read and execute workload file
    cout << "Start reading and executing workload file\n";
    executeQueryFile(lsmTree, workloadFilePath);

    return 0;
}
