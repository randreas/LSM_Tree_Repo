#include <stdlib.h>
#include <string.h>

#include "LSMTree.h"

using namespace std;

LSMTree::LSMTree(int _initial_run_size, int _num_run_per_level) {
    initial_run_size = _initial_run_size;
    num_run_per_level = _num_run_per_level;
    buffer = Run(initial_run_size);
}

LSMTree::addTuple(int index, Tuple* tuple) {
    initial_run_size = _initial_run_size;
    num_run_per_level = _num_run_per_level;
}