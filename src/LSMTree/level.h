//
// Created by jingyusu on 3/16/22.
//

#ifndef TEMPLATEDB_LEVEL_H
#define TEMPLATEDB_LEVEL_H

#include "run.h"
#include "FileMeta.h"
#include "fencePointer.h"
#include "../BloomFilter/BloomFilter.h"
#include <vector>
#include "const.h"

/** Class for levels in LSM tree
 *  Store kv pairs in txt files
 */
class Level {

private:

    //Meta data for runs in the level
    std::vector<FileMeta*> dataBlocks;

    //Clear all the dataBlocks in the level
    void clear();

    //Add a mew empty file meta
    void createAndInsertNewFileMeta();

    //Get the offset of first tuple in data file
    int getFirstTupleOffsetinBlock() const;

public:
    
    //BloomFilter per Level
    BloomFilter* bloomFilter;

    //Maximum number of dataBlocks per level

    const size_t MAX_RUN_NUM;

    const size_t MAX_TUPLE_NUM_IN_RUN;

    //Level ID: for file name generation
    unsigned int lvlID;

    FencePointer *fp;

    explicit Level(int max_run_num, int max_tuple_num_in_run, int lvlID) : MAX_RUN_NUM(max_run_num), MAX_TUPLE_NUM_IN_RUN(max_tuple_num_in_run) {
        this->dataBlocks = std::vector<FileMeta*>();
        this->lvlID = lvlID;
        this->fp = new FencePointer(max_run_num);
        //this->bloomFilter = createNewBloomFilter();
        createBloomFilter();
    }

    //If this level is saturated, no more tuples can be inserted
    bool isFull();

    //If this level is empty
    bool isEmpty();

    //Get the run at idx
    Run* getRun(int idx);

    //get meta of data block on index idx
    FileMeta* getDataMeta(int idx);

    //Get current size
    size_t getCurrentSize();

    // return the index for the filepointer, -1 if not found
    int containsKey(int key);

    //Add a file meta to the level. Happens when the upper level merges.
    void addRunFileMeta(FileMeta* fm);

    //Add a tuple, Deprecated
    //void addTuple(Tuple* tuple);

    //Get run of FileMeta at index idx
    Run* getRunByFileMetaAtIndex(int idx);

    //Merges the entire level, return pointer of a merged run
    Run* merge();

    vector<LSMTuple::Tuple*> GetAllTuples();

    void createBloomFilter();

    //Destructor of a level
    ~Level(){clear();}

    void deepClear();
};

class LevelFullException : public exception {

};


#endif //TEMPLATEDB_LEVEL_H
