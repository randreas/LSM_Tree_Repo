#include "fencePointer.h"

FencePointer::FencePointer(int _max_num_zones)  {
    max_num_zones = _max_num_zones;
    cur_num_zones = 0;
}

FencePointer::FencePointer(std::vector<Zone> _zones, int _max_num_zones)  {
    zones = _zones;
    max_num_zones = _max_num_zones;
    cur_num_zones = 0;
}

std::vector<int> FencePointer::query(int key)  {
    std::vector<int> result;
   for (int i = 0; i < cur_num_zones; i++) {
       Zone z = zones[i];
       if ((z.min <= key) && (key <= z.max)) {
           result.push_back(z.index);
       }
   }
    return result;
}

std::vector<int> FencePointer::query(int low, int high) {
    std::vector<int> result;
    cout << "fp low = " << low << " ||  high = " << high"\n";
    for (int i = cur_num_zones - 1; i >= 0; i--) {
        Zone z = zones[i];
        cout << "z.max = " << z.max << " ||  z.min = " << z.min"\n";
        if ((low > z.max) || ( high < z.min)) {
            continue;
        }
        result.push_back(z.index);
    }
    return result;
}

void FencePointer::addNewZone(FileMeta *fm) {
    if (zones.size() >= max_num_zones) {
        throw FencePointerException();
    }

    cur_num_zones++;

    Zone newZone;
    newZone.index = zones.size();
    newZone.min = fm->minKey;
    newZone.max = fm->maxKey;

    zones.push_back(newZone);
}

// shall not be used
void FencePointer::addTuple(int index, LSMTuple::Tuple* tuple) {
    throw FencePointerException();
    if (index >= zones.size()) {
        throw FencePointerException();
    }

    if (tuple->key > zones[index].max) {
        zones[index].max = tuple->key;
    }
    if (tuple->key < zones[index].min) {
        zones[index].min = tuple->key;
    }
}

void FencePointer::clear() {
    cur_num_zones = 0;
    zones.clear();
}
