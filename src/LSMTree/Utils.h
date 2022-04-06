//
// Created by jingyusu on 3/23/22.
//

#ifndef TEMPLATEDB_UTILS_H
#define TEMPLATEDB_UTILS_H

#include <sstream>
#include <fstream>
#include "FileMeta.h"
#include "itoa.h"

FileMeta *createFileMetaFromRun(size_t lvlID, size_t newBlockIdx, Run* run) {
    //TODO: Replace with binary file
    cout << "in create filemeta from run\n";
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
    } else {
        cout << "unable to open file, previous file does not exist\n";
    }

    ofstream newFile(newFilePath);
    for (Tuple* tuple : run->getTuples()) {
        string str = itoa(tuple->key) + " ";
        for (int val : tuple->value.items) {
            str += itoa(tuple->key) + " ";
        }
        str += "\n";
        newFile << str;
    }
    newFile.close();
    cout << "finished writing to new file\n";
    auto* newFileMeta = new FileMeta(newFilePath, run->MAX_TUPLE_NUM);
    cout << "created new file meta\n";
    return newFileMeta;
}

#endif //TEMPLATEDB_UTILS_H
