//
// Created by jingyusu on 3/16/22.
//

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
   for (Zone z: zones) {
       if ((z.min <= key) && (key <= z.max)) {
           result.push_back(z.index);
       }
   }
    return result;
}

std::vector<int> FencePointer::query(int low, int high) {
    std::vector<int> result;
    for (Zone z: zones) {
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

    Zone newZone;
    newZone.index = zones.size();
    newZone.min = fm->minKey;
    newZone.max = fm->maxKey;

    zones.push_back(newZone);
}

void FencePointer::addTuple(int index, Tuple* tuple) {
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
    zones.clear()
}
