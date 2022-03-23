//
// Created by xingk on 3/22/2022.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

void executeCommand(string command) {
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
        for (string cur_e : vector<string>(elements.begin() + 1, elements.end())) {
            values.push_back(stoi(cur_e));
        }
        cout << "Insert " << "key: " << key << " values: " << "\n";
        // TODO execute
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

void executeQueryFile(string filePath) {
    ifstream fs;
    fs.open(filePath);

    string line;
    if (fs.is_open()) {
        // cout << "In executeQueryFile(), file string is open.\n";
        while (getline(fs, line)) {
            // fs >> line;
            executeCommand(line);
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
    if (argc !=  4) {
        cout << "USAGE: ./main <t> <data file path> <workload file path>\n";
        return 1;
    }

    // constant, hyper parameter
    int t = stoi(argv[1]);  // t stands for the amplifier
    char* dataFilePath = argv[2];
    char* workloadFilePath = argv[3];

    // read and execute data file
    cout << "Start reading and executing data file\n";
    executeQueryFile(dataFilePath);

    // read and execute workload file
    cout << "Start reading and executing workload file\n";
    executeQueryFile(workloadFilePath);

    return 0;
}
