#include <unordered_set>
#include "LSMTree.h"
#include "run.h"
#include "Utils.h"

using namespace std;

LSMTree::LSMTree(int _initial_run_size, int _num_run_per_level, bool _isTiering) {
    initial_run_size = _initial_run_size;
    num_run_per_level = _num_run_per_level;
    isTiering = _isTiering;
    buffer = new Run(initial_run_size);
}

void LSMTree::addTuple(LSMTuple::Tuple* tuple) {
    // check if buffer is full
    // full, move run to level 1, clear buffer
    cout << "in LSMTree.addTuple\n";
    buffer->addTuple(tuple);
    cout << "Buffer after insert: \n";
    buffer->printRun();
    if (buffer->isFull()) {
        //FileMeta* bufferFile = buffer->createFileMetaFromRun(0, 0);  // level 0, index 0
        cout << "Buffer is full, need to flush\n";
        Run* push_run = new Run(buffer->MAX_TUPLE_NUM);
        push_run->merge(buffer);
        //push_run->addTuple(tuple);
        cout << "run merged\n";
        push_run->printRun();
        mergeNMove(0, push_run);
        cout << "finished merge and move\n";
        //remove(const_cast<char*>(bufferFile->filePath.c_str()));
        buffer->shallowClear();
    }
}

LSMTuple::Tuple* LSMTree::query(int key) {
    if (buffer->containsKey(key)) {
        return buffer->query(key);
    }
    //cout << "query 0\n";
    int ind;
    for (Level *curLevel: levels) {
        //cout << "query 1\n";

        //Check bloomFilter
        ind = curLevel->containsKey(key);
        //cout << "query 2\n";
        if (ind >= 0) {
            return curLevel->getRunByFileMetaAtIndex(ind)->query(key);
        }

    }

    // if not found, return a tuple with delete flag
    return new LSMTuple::Tuple(key, LSMTuple::Value(false));
}

void LSMTree::mergeNMove(int idx, Run* newRun) {
    cout << "In LSMTree.mergeNMove\n";
    moveToLevelAtIdxRecurse(idx, newRun);
    cout << "Finished LSMTree.mergeNMove\n";
}

void LSMTree::moveToLevelAtIdxRecurse(int idx, Run* newRun) {
    cout << "in move to level " << idx << "\n";
    if (idx == levels.size()) {
        cout << "here1\n";
        int newRunSize;
        if (idx == 0) {
            if (isTiering) {
                newRunSize = newRun->MAX_TUPLE_NUM;
            } else {
                newRunSize = newRun->MAX_TUPLE_NUM * num_run_per_level;
                cout << "%%%%%%%%%" << newRun->MAX_TUPLE_NUM << " " << num_run_per_level << " " << newRunSize << "\n";
            }
        } else {
            newRunSize = (levels[levels.size() - 1]->MAX_TUPLE_NUM_IN_RUN) * num_run_per_level;
        }
//        int newRunSize = idx == 0 ? newRun->MAX_TUPLE_NUM : (levels[levels.size() - 1]->MAX_TUPLE_NUM_IN_RUN) * num_run_per_level;
        //cout << newRunSize << "\n";
        int lvlId = levels.size();
        //cout << lvlId << "\n";
        levels.push_back(new Level(num_run_per_level, newRunSize, lvlId));
        //cout << levels.size() << "\n";
        levels[lvlId]->addRunFileMeta(createFileMetaFromRun(lvlId, 0, newRun));
        //cout << "level of size: " << levels.size() << "\n";
    } else {
        cout << "here2\n";
        Level *lvl = levels[idx];
        if (isTiering) {
            if (!lvl->isFull(isTiering)) {
                cout << "level " << lvl->lvlID << " is not full\n";
                lvl->addRunFileMeta(createFileMetaFromRun(idx, lvl->getCurrentSize(), newRun));
                delete newRun;
                newRun = nullptr;
            } else {
                cout << "level " << lvl->lvlID << " is full\n";
                Run* mergedResult = lvl->merge(isTiering);
                cout << "level merged\n";
                cout << "level " << lvl->lvlID << " merged result:\n";
                mergedResult->printRun();
                //mergedResult->merge(newRun);
                moveToLevelAtIdxRecurse(idx + 1, mergedResult);
                moveToLevelAtIdxRecurse(idx, newRun);
            }
        } else {
            // leveling
            cout << "leveling in mergeToLevel\n";
            Run* mergedResult = lvl->merge(isTiering);
            mergedResult->merge(newRun);
            levels[idx]->addRunFileMeta(createFileMetaFromRun(idx, 0, mergedResult));
            // full
            cout << "+++ check is full+++ \n";
            if (lvl->isFull(isTiering)) {
                cout << "+++ is full+++ \n";
                Run* moveResult = lvl->merge(isTiering);
                moveToLevelAtIdxRecurse(idx + 1, moveResult);
            }
        }
    }
}

void LSMTree::deleteKey(int key) {
    addTuple(new LSMTuple::Tuple(key, LSMTuple::Value(false)));
}

void LSMTree::writeBufferToFile() const {
    writeRunToFileWithPath("buffer.bin", buffer);
}

void LSMTree::readBufferFromFile() {
    string bufferPath = "buffer.bin";
    char* path = const_cast<char*>(bufferPath.c_str());
    ifstream newFile(path, ios::in|ios::binary);
    readRunFromData(&newFile, buffer);
}

//Called when restart
void LSMTree::open() {
    vector<int> parameters = readMetaDataFromFile();
    if (parameters.empty()) {
        return;
    }
    delete buffer;
    buffer = new Run(initial_run_size);
    readBufferFromFile();
    int lvlCnt = parameters[0];
    for (int i = 0; i < lvlCnt; i++) {
        int lvlSize = parameters[2 * i + 1], max_tuple_num_in_run = parameters[2 * i + 2];
        auto* lvl = new Level(num_run_per_level, max_tuple_num_in_run, i);
        for (int j = 0; j < lvlSize; j++) {
            lvl->addRunFileMeta(createFileMetaFromExistingFile(i, j, max_tuple_num_in_run));
        }
        levels.push_back(lvl);
    }
}

void LSMTree::close() {
    writeMetaDataToFile();
    writeBufferToFile();
    for (Level* lvl : levels) {
        delete lvl;
    }
    levels.clear();
}

vector<int> LSMTree::readMetaDataFromFile() {
    string fileName = "metaData.bin";
    char* path = const_cast<char*>(fileName.c_str());
    if (!(fopen(path, "r"))) {
        cout << "No previous meta data!\n";
        return {};
    } else {
        ifstream metaDataFile(path, ios::in|ios::binary);
        int lvlCnt;
        int offSet = 0;
        readIntFromOffset(&metaDataFile, &offSet, &lvlCnt);
        vector<int> parameters;
        parameters.push_back(lvlCnt);
        readIntFromOffset(&metaDataFile, &offSet, &initial_run_size);
        readIntFromOffset(&metaDataFile, &offSet, &num_run_per_level);
        for (int i = 0; i < lvlCnt; i++) {
            int lvlSize;
            readIntFromOffset(&metaDataFile, &offSet, &lvlSize);
            parameters.push_back(lvlSize);
            int blockSize;
            readIntFromOffset(&metaDataFile, &offSet, &blockSize);
            parameters.push_back(blockSize);
        }
        char isTieringIndicator;
        readCharFromOffset(&metaDataFile, &offSet, &isTieringIndicator);
        isTiering = isTieringIndicator == '1';
        return parameters;
    }
}

void LSMTree::writeMetaDataToFile() {
    string fileName = "metaData.bin";
    char* path = const_cast<char*>(fileName.c_str());
    if (FILE *file = fopen(path, "r")) {
        fclose(file);
        remove(path);
    } else {
        cout << "unable to open file, previous meta data file does not exist\n";
    }
    ofstream metaDataFile(path, ios::out|ios::binary);
    int offSet = 0;
    int lvlCnt = levels.size();
    //Number of levels
    writeIntToOffset(&metaDataFile, &offSet, lvlCnt);
    //Buffer Capability
    writeIntToOffset(&metaDataFile, &offSet, initial_run_size);
    //Merge threshold
    writeIntToOffset(&metaDataFile, &offSet, num_run_per_level);
    for (Level* lvl : levels) {
        int lvlSize = lvl->getCurrentSize();
        writeIntToOffset(&metaDataFile, &offSet, lvlSize);
        int max_tuple_in_run = lvl->MAX_TUPLE_NUM_IN_RUN;
        writeIntToOffset(&metaDataFile, &offSet, max_tuple_in_run);
    }
    char tieringIndicator = isTiering ? '1' : '0';
    writeCharToOffset(&metaDataFile, &offSet, tieringIndicator);
}

void LSMTree::writeCharToOffset(ofstream* fileStream, int* offSet, char data) {
    fileStream->seekp(*offSet);
    fileStream->write(reinterpret_cast<char*>(&data),SIZE_OF_CHAR);
    *offSet += SIZE_OF_CHAR;
}

void LSMTree::readCharFromOffset(ifstream* fileStream, int* offSet, char* data) {
    fileStream->seekg(*offSet);
    fileStream->read(reinterpret_cast<char*>(data),SIZE_OF_CHAR);
    *offSet += SIZE_OF_CHAR;
}

void LSMTree::writeIntToOffset(ofstream* fileStream, int* offSet, int data) {
    fileStream->seekp(*offSet);
    fileStream->write(reinterpret_cast<char*>(&data),SIZE_OF_INT);
    *offSet += SIZE_OF_INT;
}

void LSMTree::readIntFromOffset(ifstream* fileStream, int* offSet, int* data) {
    fileStream->seekg(*offSet);
    fileStream->read(reinterpret_cast<char*>(data),SIZE_OF_INT);
    *offSet += SIZE_OF_INT;
}

int LSMTree::getLevelCnt() {
    return levels.size();
}

void LSMTree::deleteKey(int low, int high) {
    vector<LSMTuple::Tuple*> toBeDeletedList = query(low,high);

    for(LSMTuple::Tuple* t : toBeDeletedList) {
        cout << "Key to be deleted " << t->key << "\n";
        addTuple(new LSMTuple::Tuple(t->key, LSMTuple::Value(false)));
    }
}

void LSMTree::printLSMTree() {
    cout << "++++++++++++++++++++++++\n";
    cout << "Current LSM Tree Status: \n";
    if (isTiering) {
        cout << "Policy: Tiering \n";
    } else {
        cout << "Policy: Leveling \n";
    }
    cout << "Buffer: \n";
    buffer->printRun();
    cout << "Levels: \n";
    for (Level* lvl : levels) {
        lvl->printLevel();
    }
    cout << "++++++++++++++++++++++++\n";
}


vector<LSMTuple::Tuple*> LSMTree::query(int low, int high) {
    vector<LSMTuple::Tuple*> result;
    unordered_set <int> set;
    cout << "QUERY RANGE START \n";
    for(LSMTuple::Tuple* t : buffer->getTuples()) {
        int key = t-> key;
        cout << " buffer tuples curr key scan = " << key << "\n";
        if(key <= high && key >= low) {
            cout << "curr key in range \n";
            if(set.find(key) != set.end()) {
                cout << "key already is in the set" << "\n";
            } else {
                cout << "new key found adding into result" << "\n";
                set.insert(t->key);
                if(! t->isDeleteMarker()) {
                    result.push_back(t);
                }
                
            }
        }
    }

    for (Level *curLevel: levels) {
        cout << " levels curr key scan \n";
        vector<int> zoneIdxs = curLevel->fp->query(low, high);
        cout << " size of zoneIdxs = " << zoneIdxs.size() << "\n";
        for(int idx : zoneIdxs) {
            Run* currRun = curLevel->getRunByFileMetaAtIndex(idx);
            vector<LSMTuple::Tuple*> curr_tuples = currRun->getTuples();
            for(LSMTuple::Tuple* t : curr_tuples) {
                int key = t->key;
                if(key <= high && key >= low) {
                    if(set.find(key) != set.end()) {
                        cout << "key already is in the set" << "\n";
                    } else {
                        cout << "new key found adding into result"<< "\n";
                        set.insert(t->key);
                        if(! t->isDeleteMarker()) {
                            result.push_back(t);
                        }
                    }
                }
            }
        }
        
    }
    return result;
}
