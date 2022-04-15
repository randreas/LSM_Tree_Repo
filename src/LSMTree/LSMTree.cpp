#include "LSMTree.h"
#include "run.h"
#include "Utils.h"

using namespace std;

LSMTree::LSMTree(int _initial_run_size, int _num_run_per_level) {
    initial_run_size = _initial_run_size;
    num_run_per_level = _num_run_per_level;
    buffer = new Run(initial_run_size);
}

void LSMTree::addTuple(LSMTuple::Tuple* tuple) {
    // check if buffer is full
    // full, move run to level 1, clear buffer
    cout << "in add tuple\n";
    if (buffer->isFull()) {
        //FileMeta* bufferFile = buffer->createFileMetaFromRun(0, 0);  // level 0, index 0
        cout << "Buffer is full, need to flush\n";
        Run* push_run = new Run(buffer->MAX_TUPLE_NUM + 1);
        push_run->merge(buffer);
        push_run->addTuple(tuple);
        cout << "run merged\n";
        push_run->printRun();
        mergeNMove(0, push_run);
        //cout << "finished merge and move\n";
        //remove(const_cast<char*>(bufferFile->filePath.c_str()));
        buffer->shallowClear();
    } else {
        buffer->addTuple(tuple);
        cout << "Buffer after insert: \n";
        buffer->printRun();
    }
}

LSMTuple::Tuple* LSMTree::query(int key) {
    if (buffer->containsKey(key)) {
        return buffer->query(key);
    }
    cout << "query 0\n";
    int ind;
    for (Level *curLevel: levels) {
        cout << "query 1\n";

        //RA todo
        //Check bloomFilter
        ind = curLevel->containsKey(key);
        cout << "query 2\n";
        if (ind >= 0) {
            return curLevel->getRunByFileMetaAtIndex(ind)->query(key);
        }
        cout << "query 3\n";

    }

    // if not found, return a tuple with delete flag
    return new LSMTuple::Tuple(key, LSMTuple::Value(false));
}

void LSMTree::mergeNMove(int idx, Run* newRun) {
    moveToLevelAtIdxRecurse(idx, newRun);
}

void LSMTree::moveToLevelAtIdxRecurse(int idx, Run* newRun) {
    cout << "in move to level\n";
    if (idx == levels.size()) {
        cout << "here1\n";
        int newRunSize = idx == 0 ? newRun->MAX_TUPLE_NUM : (levels[levels.size() - 1]->MAX_TUPLE_NUM_IN_RUN + 1) * num_run_per_level;
        //cout << newRunSize << "\n";
        int lvlId = levels.size();
        //cout << lvlId << "\n";
        levels.push_back(new Level(num_run_per_level, newRunSize, lvlId));
        //cout << levels.size() << "\n";
        levels[lvlId]->addRunFileMeta(createFileMetaFromRun(lvlId, 0, newRun));
        cout << "level of size: " << levels.size() << "\n";
    } else {
        cout << "here2\n";
        Level *lvl = levels[idx];
        if (!lvl->isFull()) {
            lvl->addRunFileMeta(createFileMetaFromRun(idx, lvl->getCurrentSize(), newRun));
        } else {
            cout << "level " << lvl->lvlID << " is full\n";
            Run* mergedResult = lvl->merge();
            cout << "level merged\n";
            cout << "level " << lvl->lvlID << " merged result:\n";
            mergedResult->printRun();
            mergedResult->merge(newRun);
            moveToLevelAtIdxRecurse(idx + 1, mergedResult);
        }
    }
    delete newRun;
    newRun = nullptr;
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
        return parameters;
    }
}
