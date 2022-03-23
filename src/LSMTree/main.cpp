//
// Created by xingk on 3/22/2022.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fstream>

using namespace std;

void executeQueryFile(string filePath){
    ifstream fs;
    fs.open(filePath);

    string line;
    if (fs.is_open()) {
        cout << "In executeQueryFile(), file string is open.\n";
        while (fs.good()) {
            fs >> line;
            cout << line << "\n";
        }
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
