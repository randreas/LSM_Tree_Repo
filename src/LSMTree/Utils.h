//
// Created by jingyusu on 3/23/22.
//

#ifndef TEMPLATEDB_UTILS_H
#define TEMPLATEDB_UTILS_H

#include <sstream>
#include <fstream>
#include "FileMeta.h"
#include "itoa.h"
#include "tuple.h"

const int SIZE_OF_INT = sizeof(int);

void writeRunToFileWithPath(const string& newFilePath, Run* run) {
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
    int tupleOffSet = SIZE_OF_INT * (2 + run->MAX_TUPLE_NUM);
    //Write first
    newFile.seekp(SIZE_OF_INT);
    newFile.write(reinterpret_cast<char*>(&tupleOffSet),SIZE_OF_INT);
    for (int i = 0; i < size; i++) {
        LSMTuple::Tuple* tuple = run->getTuples()[i];
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
        //Write first
        newFile.seekp(SIZE_OF_INT * (i + 2));
        newFile.write(reinterpret_cast<char*>(&tupleOffSet),SIZE_OF_INT);
    }
    newFile.close();
    cout << "finished writing to new file\n";
}

void readRunFromData(ifstream* inFile, Run* run) {
    int tupleCnt;
    inFile->read(reinterpret_cast<char *>(&tupleCnt), SIZE_OF_INT);
    for (int i = 0; i < tupleCnt; i++) {
        cout << "tuple " << i << "\n";
        inFile->seekg(sizeof(int) * (i + 1));
        int offset;
        inFile->read(reinterpret_cast<char *>(&offset), SIZE_OF_INT);
        inFile->seekg(sizeof(int) * (i + 2));
        int nextTupleOffset;
        inFile->read(reinterpret_cast<char *>(&nextTupleOffset), SIZE_OF_INT);
        cout << "offset: " << offset << ", next offset: " << nextTupleOffset << "\n";
        inFile->seekg(offset);
        //Read key from file
        int key;
        inFile->read(reinterpret_cast<char *>(&key), SIZE_OF_INT);
        offset += SIZE_OF_INT;
        //Read val items from file
        LSMTuple::Value val;
        while (offset < nextTupleOffset) {
            int valItem;
            inFile->seekg(offset);
            inFile->read(reinterpret_cast<char *>(&valItem), SIZE_OF_INT);
            val.items.push_back(valItem);
            offset += SIZE_OF_INT;
        }
        auto* newTuple = new LSMTuple::Tuple(key, val);
        run->addTuple(newTuple);
    }
    cout << "generated run: \n";
    run->printRun();
    inFile->close();
}

FileMeta *createFileMetaFromRun(size_t lvlID, size_t newBlockIdx, Run* run) {
    cout << "in create filemeta from run\n";
    stringstream ss;
    ss << "level-";
    ss << lvlID;
    ss << "-block-";
    ss << newBlockIdx;
    ss << ".bin";
    string newFilePath = ss.str();
    writeRunToFileWithPath(newFilePath, run);
    auto* newFileMeta = new FileMeta(newFilePath, run->MAX_TUPLE_NUM);
    newFileMeta->minKey = run->getTuples()[0]->getKey();
    newFileMeta->maxKey = run->getTuples()[run->getSize() - 1]->getKey();
    cout << "created new file meta\n";
    return newFileMeta;
}

FileMeta* createFileMetaFromExistingFile(size_t lvlID, size_t blockIdx, int max_tuple_num) {
    stringstream ss;
    ss << "level-";
    ss << lvlID;
    ss << "-block-";
    ss << blockIdx;
    ss << ".bin";
    string filePath = ss.str();
    cout << "in create filemeta from existing file " << filePath << "\n";
    auto* newFileMeta = new FileMeta(filePath, max_tuple_num);
    char* path = const_cast<char*>(filePath.c_str());
    ifstream newFile(path, ios::in|ios::binary);
    int tupleCnt;
    newFile.read(reinterpret_cast<char *>(&tupleCnt), SIZE_OF_INT);

    newFile.seekg(SIZE_OF_INT);
    int minKeyOffSet;
    newFile.read(reinterpret_cast<char *>(&minKeyOffSet), SIZE_OF_INT);

    newFile.seekg(minKeyOffSet);
    int minKey;
    newFile.read(reinterpret_cast<char *>(&minKey), SIZE_OF_INT);

    int maxKeyOffsetOffset = tupleCnt;
    newFile.seekg(maxKeyOffsetOffset);
    int maxKeyOffset;
    newFile.read(reinterpret_cast<char *>(&maxKeyOffset), SIZE_OF_INT);

    newFile.seekg(maxKeyOffset);
    int maxKey;
    newFile.read(reinterpret_cast<char *>(&maxKey), SIZE_OF_INT);

    newFileMeta->minKey = minKey;
    newFileMeta->maxKey = maxKey;
    cout << "created new file meta from existing file " << filePath << "\n";
    return newFileMeta;
}


#endif //TEMPLATEDB_UTILS_H
