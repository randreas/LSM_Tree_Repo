//
// Created by jingyusu on 3/16/22.
//

#include "level.h"
#include "run.h"
#include "const.h"
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include "../BloomFilter/BloomFilter.h"

bool Level::isFull() {
    cout << "In level.isFull()\n";
    cout << dataBlocks.size() << "\n";
    cout << MAX_RUN_NUM << "\n";
    return dataBlocks.size() == MAX_RUN_NUM;
}

Run *Level::getRun(int idx) {
    return dataBlocks[idx]->getRun();
}

size_t Level::getCurrentSize() {
    return dataBlocks.size();
}

/* Deprecated
void Level::addTuple(Tuple *tuple) {
    if (isFull()) {
        throw LevelFullException();
    }
    if (isEmpty()) {
        createAndInsertNewFileMeta();
        dataBlocks[0]->appendTupleToFile(tuple);
    } else {
        if (dataBlocks[dataBlocks.size() - 1]->isFull()) {
            createAndInsertNewFileMeta();
        }
        dataBlocks[dataBlocks.size() - 1]->appendTupleToFile(tuple);
    }
}
 */

int Level::containsKey(int key) {
    cout << "containsKey 0\n";
    if (!this->bloomFilter->query(reinterpret_cast<const char *>(&key))) {
        return -1;
    }
    //cout << "containsKey 1\n";
    std::vector<int> possibleZones = fp->query(key);
    if (possibleZones.size() == 0) {
        return -1;
    }
    //cout << "containsKey 2\n";
    for (int i : possibleZones) {
        Run* curRun = getRunByFileMetaAtIndex(i);
        if (curRun->containsKey(key)) {
            return i;
        }
    }
    //cout << "containsKey 3\n";
    return -1;
}


Run *Level::merge() {
    cout << "in level.merge\n";
    Run* initRun = new Run((MAX_RUN_NUM + 1) * MAX_TUPLE_NUM_IN_RUN);
    cout << "created new run\n";
    for (FileMeta* fm : dataBlocks) {
        initRun->merge(fm->getRun());
        cout << "MERGED " << fm->getFilePath() << "\n";
        cout << "Merged result: \n";
        initRun->printRun();
    }
    cout << "merged\n";
    deepClear();
    cout << "cleared\n";
    fp->clear();
    //RA todo recreate bloomfilter
    createBloomFilter();
    return initRun;
}

void Level::deepClear() {
    while (!dataBlocks.empty()) {
        FileMeta* fmToDel = dataBlocks.back();
        fmToDel->clearFiles();
        dataBlocks.pop_back();
        delete fmToDel;
        fmToDel = nullptr;
    }
}

void Level::clear() {
    while (!dataBlocks.empty()) {
        FileMeta* fmToDel = dataBlocks.back();
        dataBlocks.pop_back();
        delete fmToDel;
        fmToDel = nullptr;
    }
}

bool Level::isEmpty() {
    return dataBlocks.empty();
}

FileMeta *Level::getDataMeta(int idx) {
    return dataBlocks[idx];
}

void Level::addRunFileMeta(FileMeta *fm) {
    cout << "in level.addRunFileMeta\n";
    if (!isFull()) {
        //cout << "here1\n";
        dataBlocks.push_back(fm);
        fp->addNewZone(fm);
    } else {
        //cout << "here2\n";
        throw LevelFullException();
    }

    //RA todo recreate bloomfilter
    createBloomFilter();
    cout << "Added FileMeta " << fm->filePath << "to level "<< lvlID << "\n";
}

Run *Level::getRunByFileMetaAtIndex(int idx) {
    return getDataMeta(idx)->getRun();
}

//32 bit cnt, max#of tuples offset
void Level::createAndInsertNewFileMeta() {
    stringstream ss;
    ss << "./";
    ss << "level-";
    ss << lvlID;
    ss << "-block-";
    ss << dataBlocks.size();
    ss << ".bin";
    string newFilePath = ss.str();
    ofstream newFile(newFilePath, ios::out|ios::binary);
    int tupleCnt = 0;
    newFile << tupleCnt;
    newFile << getFirstTupleOffsetinBlock();
    newFile.close();
    addRunFileMeta(new FileMeta(newFilePath, MAX_TUPLE_NUM_IN_RUN));
}

int Level::getFirstTupleOffsetinBlock() const {
    return sizeof(int) * (1 + MAX_TUPLE_NUM_IN_RUN);
}

//RA Todo
vector<LSMTuple::Tuple*> Level::GetAllTuples() {
    vector<LSMTuple::Tuple*> result;
    for (auto & dataBlock : dataBlocks) {
//        DEBUG_LOG(std::string("getting tuples from #") + dataBlock->getFilePath());
        auto tuples = dataBlock->GetAllTuples();
        result.insert(result.end(), tuples.begin(), tuples.end());
    }
    return result;
}


////RA Todo
void Level::createBloomFilter() {
    cout << "In create bloom filter\n";
    auto* bf = new BloomFilter(BF_NUM_TUPLES, BF_BITS_PER_ELEMENT);
    vector<LSMTuple::Tuple*> tupleList = GetAllTuples();
    for(LSMTuple::Tuple* t : tupleList) {
//        if (t == nullptr) {
//            cout << "tuple is null!\n";
//        } else {
//            cout << "tuple key: " << t->key << "\n";
//        };
        int key = t->key;
//        cout << reinterpret_cast<const char *>(&key) << "\n";
        bf->program(reinterpret_cast<const char *>(&key));
    }
    cout << "finished Bloom Filter creation, programmed all tuples\n";
    bloomFilter = bf;
}
