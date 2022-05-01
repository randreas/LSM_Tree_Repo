
//
// Created by xingk on 3/22/2022.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "./LSMTree.h"
#include "./tuple.h"

class KeyException : public exception {};

using namespace std;

void printIntVector(vector<int> v) {
    for (int e : v) {
        cout << e << " ";
    }
}

void executeCommand(LSMTree* lsmTree, string command, string outputFilePath) {
    
    stringstream iss(command);
    vector<string> elements;
    string e;


    ofstream fw(outputFilePath, std::ios_base::app);

    
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
       // cout << "---------------------------------------\n";
        cout << "Insert " << "key: " << key << " values: ";
        printIntVector(values);
        cout << "\n";
        // execute
        lsmTree->addTuple(new LSMTuple::Tuple(key, LSMTuple::Value(values)));
    //    cout << "Finished adding LSMTree.addTuple with key " << key << "\n";
        //lsmTree->buffer->printRun();
        //if (lsmTree->buffer == nullptr) {
        //    cout << "buffer is null\n";
        //}


        if (fw.is_open()) {
            fw << "Insert " << key << "\n";
        }
        fw.close();
    } else if (elements[0] == "Q") {
        if (elements.size() != 2) {
            cout << "Q with incorrect size\n";
            return;
        }
        int key = stoi(elements[1]);
        // cout << "---------------------------------------\n";
        // cout << "Point query key: " << key << "\n";
        // execute
        LSMTuple::Tuple* resultTuple = lsmTree->query(key);
        if (resultTuple->key != key) {
            throw KeyException();
        }
        if (resultTuple->isDeleteMarker()) {
            cout << "query result : key: " << key << " not in the lsm tree, not entered or deleted" << "\n";
            if (fw.is_open()) {
                fw << "Did not find " << key << "\n";
            }
            fw.close();
        } else {
            cout << "query result : key: " << key << " value: ";
            resultTuple->getValue().printValue();
            cout<< "\n";

            if (fw.is_open()) {
                fw << "Found " << key << "\n";
            }
            fw.close();
        }
    } else if (elements[0] == "S") {
        if (elements.size() != 3) {
            cout << "S with incorrect size\n";
            return;
        }
        int key_low = stoi(elements[1]);
        int key_high = stoi(elements[2]);
      //  cout << "Range query " << "low key: " << key_low << " high key: " << key_high << "\n";


        vector<LSMTuple::Tuple*> resultTuples = lsmTree->query(key_low, key_high);
        vector<int> keyList;


        if(resultTuples.size() <= 0) {
            if (fw.is_open()) {
                fw << "Did not find rangeScan  [" << elements[1] << ", " << elements[2] << "]\n" ; 
                cout << "Did not find rangeScan  [" << elements[1] << ", " << elements[2] << "]\n" ; 
            }
        } else {
            cout << "Found rangeScan [\n";
            for(LSMTuple::Tuple* t : resultTuples) {
                    keyList.push_back(t->key);
                    cout << "\t key: " << t->key << " value: ";
                    t->getValue().printValue();
                    cout<< "\n";

            }
            cout << "]\n";
            std::sort(keyList.begin(), keyList.end());
            if (fw.is_open()) {
                if(keyList.size() > 0) {
                    fw << "Found rangeScan [";
                    for (int key : keyList) {
                        fw << key << " ";
                    }
                    fw << "]\n";
                } 
            }

        }
       
        
        fw.close();



    } else if (elements[0] == "D") {
        if (elements.size() == 2) {
            int key = stoi(elements[1]);
            lsmTree->deleteKey(key);
           
            cout << "Deleted " << key << "\n";
            if (fw.is_open()) {
                fw << "Deleted " << key << "\n";
            }
            fw.close();

        } else if (elements.size() == 3) {
            int low = stoi(elements[1]);
            int high = stoi(elements[2]);
            // cout << "---------------------------------------\n";
           
            // execute
            vector<LSMTuple::Tuple*> toBeDeletedList = lsmTree->deleteKey(low,high);
            vector<int> keyList;

            for(LSMTuple::Tuple* t : toBeDeletedList) {
                keyList.push_back(t->key);
            }
            std::sort(keyList.begin(), keyList.end());
            if (fw.is_open()) {
                fw << "Deleted: [";
                cout << "Deleted: [";
                for(int key : keyList) {
                    fw << key << " ";
                    cout << key << " ";
                }
                fw << "]\n";
                cout << "]\n";
            }
            fw.close();

            // cout << "delete; addtuple finished\n";
        } else {
            // cout << "D with incorrect size\n";
            return;
        }
    }
}

void executeQueryFile(LSMTree* lsmTree, string filePath, string outputFilePath) {
    ifstream fs;
    fs.open(filePath);

    string line;
    if (fs.is_open()) {
        // cout << "In executeQueryFile(), file string is open.\n";
        while (getline(fs, line)) {
            // fs >> line;
            executeCommand(lsmTree, line, outputFilePath);
        }
        // while (fs.good()) {
        //     fs >> line;
        //     cout << line << "\n";
        // }
    } else {
        // cout << "In executeQueryFile(), file string is close.\n";
    }
}

void commandLineHelp() {
    cout << "Start reading command from terminal\n";
    cout << "   'exit' for exit\n";
    cout << "   'help' for display this information again\n";
    cout << "   'status' for display current LSM Tree status\n";
    cout << "   'I K V...' for inserting key value pair, number of V should be the same as the length specified above\n";
    cout << "   'Q K' for querying the value of the specified key\n";
    cout << "   'S minK maxK' for querying the value of the keys between minK and maxK\n";
    cout << "   'D K [max_K]' for deleting the specified key, 2 parameters for range delete\n";
    cout << "*** invalid input will cause error\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argc >  3) {
        cout << "USAGE: ./main  <outputFilePath> or ./main <outputFilePath> <data file path> \n";
        return 1;
    }

    // constant, hyper parameter
    int initial_run_size = 3;
    int num_run_per_level = 3;
    bool isTiering = false;

    // constant, hyper parameter
    char* outputFilePath = argv[1];
    
    

    // create levels for this tree
    LSMTree* lsmTree = new LSMTree(initial_run_size, num_run_per_level, isTiering);
    lsmTree->open();
    cout << "LSMTREE OPEN finished\n";
    if (lsmTree->isTiering) {
        cout << "Tiering Tree\n";
    } else {
        cout << "Leveling Tree\n";
    }

    if (argc > 2) {
        char* inputFilePath = argv[2];
        // read and execute data file
        cout << "buffer: \n";
        lsmTree->buffer->printRun();
        cout << "tree level number: " << lsmTree->getLevelCnt() << "\n";
        cout << "Start reading and executing data file\n";
        executeQueryFile(lsmTree, inputFilePath, outputFilePath);
    }

    commandLineHelp();
    string command = "tmp";
    // start taking command line input
    while (command != "exit") {
        cout << "[command input]>>> ";
        getline (cin, command);
        if (command == "help") {
                commandLineHelp();
                continue;
        }
        if (command == "status") {
            lsmTree->printLSMTree();
            continue;
        }
        executeCommand(lsmTree, command, outputFilePath);
    }

    lsmTree->close();

    return 0;
}
