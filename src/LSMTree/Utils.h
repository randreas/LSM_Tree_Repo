//
// Created by jingyusu on 3/23/22.
//

#ifndef TEMPLATEDB_UTILS_H
#define TEMPLATEDB_UTILS_H

#include <sstream>
#include <fstream>
#include "FileMeta/FileMeta.h"
#include "itoa.h"

FileMeta *createFileMetaFromRun(size_t lvlID, size_t newBlockIdx, Run* run) {
    //TODO: Replace with binary file
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
    auto* newFileMeta = new FileMeta(newFilePath, run->MAX_TUPLE_NUM);
    newFile.close();
    return newFileMeta;
}

#endif //TEMPLATEDB_UTILS_H
