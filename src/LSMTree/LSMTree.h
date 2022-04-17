#ifndef LSMTREE_H
#define LSMTREE_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <cmath>

#include "run.h"
#include "level.h"
#include "tuple.h"
#include "Utils.h"

class LSMTree
{
    int initial_run_size, num_run_per_level;
    vector<Level*> levels;

public:
    bool isTiering;
    Run* buffer;

    LSMTree(int _initial_run_size, int _num_run_per_level, bool _isTiering);

    /**
     * Return all index of file meta that contains the key, null if the key does not exist in this fence pointer
     */
    LSMTuple::Tuple* query(int key);

    /**
     * Given a high and a low, return all index of file meta that contains key in the range
     * @param low
     * @param high
     * @return
     */
    vector<LSMTuple::Tuple*> query(int low, int high);

    void addTuple(LSMTuple::Tuple* tuple);

    void deleteKey(int key);

    //Opens the index and load previous data
    void open();

    //Persist data to files
    void close();

    //Destructor
    ~LSMTree(){close();}

    int getLevelCnt();
  
    void deleteKey(int low, int high);


private:
    void mergeNMove(int sourceLevel, Run* newRun);

    void moveToLevelAtIdxRecurse(int idx, Run *newRun);

    void writeBufferToFile() const;

    void writeMetaDataToFile();

    vector<int> readMetaDataFromFile();

    static void writeIntToOffset(ofstream* fileStream, int *offSet, int data);

    static void readIntFromOffset(ifstream *fileStream, int *offSet, int *data);

    void readBufferFromFile();

};

#endif