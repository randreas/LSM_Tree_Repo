//
// Created by jingyusu on 3/23/22.
//

#ifndef TEMPLATEDB_UTILS_H
#define TEMPLATEDB_UTILS_H

#include <sstream>
#include <fstream>
#include "FileMeta.h"
#include "itoa.h"

const int SIZE_OF_INT = sizeof(int);

FileMeta *createFileMetaFromRun(size_t lvlID, size_t newBlockIdx, Run* run) {
    //TODO: Replace with binary file
    cout << "in create filemeta from run\n";
    stringstream ss;
    ss << "level-";
    ss << lvlID;
    ss << "-block-";
    ss << newBlockIdx;
    ss << ".bin";
    string newFilePath = ss.str();
    char* path = const_cast<char*>(newFilePath.c_str());

    if (FILE *file = fopen(path, "r")) {
        fclose(file);
        remove(path);
    } else {
        cout << "unable to open file, previous file does not exist\n";
    }

    ofstream newFile(newFilePath, ios::out|ios::binary);
    //Write # of tuples
    int size = run->getSize();
    newFile.write(reinterpret_cast<char*>(&size),SIZE_OF_INT);
    int tupleOffSet = SIZE_OF_INT * (1 + run->MAX_TUPLE_NUM);
    for (int i = 0; i < size; i++) {
        Tuple* tuple = run->getTuples()[i];
        //Write tuple offset
        newFile.seekp(SIZE_OF_INT * (i + 1));
        newFile.write(reinterpret_cast<char*>(&tupleOffSet),SIZE_OF_INT);
        //Write key
        int key = tuple->key;
        newFile.seekp(tupleOffSet);
        newFile.write(reinterpret_cast<char*>(&key),SIZE_OF_INT);
        tupleOffSet += SIZE_OF_INT;
        for (int val : tuple->value.items) {
            newFile.seekp(tupleOffSet);
            newFile.write(reinterpret_cast<char*>(&val),SIZE_OF_INT);
            tupleOffSet += SIZE_OF_INT;
        }
    }
    newFile.close();
    cout << "finished writing to new file\n";
    auto* newFileMeta = new FileMeta(newFilePath, run->MAX_TUPLE_NUM);
    cout << "created new file meta\n";
    return newFileMeta;
}

#endif //TEMPLATEDB_UTILS_H
