#include <stdlib.h>
#include <string.h>

using namespace std;

LSMTree::LSMTree(int _initial_run_size, int _num_run_per_level) {
    initial_run_size = _initial_run_size;
    num_run_per_level = _num_run_per_level;
    buffer = Run(initial_run_size);
}

LSMTree::addTuple(Tuple* tuple) {
    // check if buffer is full
    // full, move run to level 1, clear buffer
    if (buffer.isFull()) {
        FileMeta* bufferFile = buffer.createFileMetaFromRun(0, 0);  // level 0, index 0
        mergeNMove(0);
        remove(bufferFile->filePath);
        buffer.clear();
    }

    // add tuple
    buffer.addTuple(tuple);
}

Tuple* LSMTree::query(int key) {

    // if not found, return a tuple with delete flag
    return Tuple(key, Value(false));
}