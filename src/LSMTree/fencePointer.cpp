#include "fencePointer.h"

FencePointer::FencePointer(int _num_zones)  {
    num_zones = _num_zones;
}

FencePointer::FencePointer(std::vector<Zone> _zones, int _num_zones)  {
    zones = _zones;
    num_zones = _num_zones;
}

std::vector<int> FencePointer::query(int key)  {
    std::vector<int> result;
   for (int i = 0; i < num_zones; i++) {
       Zone z = zones[i];
       if ((z.min <= key) && (key <= z.max)) {
           result.push_back(z.index);
       }
   }
    return result;
}

std::vector<int> FencePointer::query(int low, int high) {
    std::vector<int> result;
    for (int i = num_zones - 1; i >= 0; i--) {
        Zone z = zones[i];
        if ((low < z.min) || (z.max < high)) {
            continue;
        }
        result.push_back(z.index);
    }
    return result;
}

void FencePointer::addNewZone(FileMeta *fm) {
    if (zones.size() >= num_zones) {
        throw FencePointerException;
    }

    num_zones++;

    Zone newZone;
    newZone.index = zones.size();
    newZone.min = fm->minKey;
    newZone.max = fm->maxKey;

    zones.push_back(newZone);
}

// shall not be used
void FencePointer::addTuple(int index, Tuple* tuple) {
    throw FencePointerException;
    if (index >= zones.size()) {
        throw FencePointerException;
    }

    if (tuple->key > zones[i].max) {
        zones[i].max = tuple->key;
    }
    if (tuple->key < zones[i].min) {
        zones[i].min = tuple->key;
    }
}

void FencePointer::clear() {
    num_zones = 0;
    zones.clear()
}
