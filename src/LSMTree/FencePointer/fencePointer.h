//
// Created by jingyusu on 3/16/22.
//

#ifndef ZONEMAPS_H
#define ZONEMAPS_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
#include <cmath>

#include "../tuple.h"

struct Zone
{
    int index;
    int min;
    int max;
};


class FencePointer
{
    std::vector<Zone> zones;
    int num_zones;

public:

    FencePointer(int _num_zones);

    /**
     * Constructor
     * @param _zones
     * @param _num_zones
     */
    FencePointer(std::vector<Zone> _zones, int _num_zones);

    /**
     * Return all index of file meta that contains the key, null if the key does not exist in this fence pointer
     */
    std::vector<int> query(int key);

    /**
     * Given a high and a low, return all index of file meta that contains key in the range
     * @param low
     * @param high
     * @return
     */
    std::vector<int> query(int low, int high);

    void addNewZone(FileMeta *fm);

    /**
     * Insert a new tuple into the fence pointer.
     */
    void addTuple(int index, Tuple* tuple);

    /**
     * clear runs of the levl
     */
    void clear();

};

class FencePointerException : public exception {};

#endif