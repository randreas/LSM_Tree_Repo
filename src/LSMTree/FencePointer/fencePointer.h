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
    *Run run;
    int min;
    int max;
    int size;
};


class FencePointer
{
    std::vector<Zone> zones;
    int num_zones;

public:
    /**
     * Constructor
     * @param _zones
     * @param _num_zones
     */
    FencePointer(std::vector<Zone> _zones, int _num_zones);

    /**
     * Return the pointer of the run that contains the key, null if the key does not exist in this fence pointer
     */
    *Run query(int key);

    /**
     * Given a high and a low, return all runs that contains key in the range
     * @param low
     * @param high
     * @return
     */
    std::vector<*Run> query(T low, T high);

    /**
     * Insert a new tuple into the fence pointer.
     */
    *Run addTuple(Tuple* newTuple);

    /**
     * Merge the runs on this level
     */
    merge();

    /**
     * Move the run to the next level
     */
    move();


};
#endif