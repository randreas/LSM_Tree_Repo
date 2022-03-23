//
// Created by xingk on 3/22/2022.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fstream>

using namespace std;

void executeQueryFile(ifstream* fs){
    string line;
    while (getline(*fs, line)) {
        cout << line;
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
    ifstream dataFile;
    dataFile.open(dataFilePath);
    executeQueryFile(&dataFile);
    dataFile.close();

    cout << "Hello World";

    // read and execute workload file
    ifstream workloadFile;
    workloadFile.open(workloadFilePath);
    executeQueryFile(&workloadFile);
    workloadFile.close();

    return 0;
}
