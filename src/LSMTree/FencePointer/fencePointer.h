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

template <typename T>
struct Zone
{
    Run* run;
    T min;                   // minimum element in the zone
    T max;                   // maximum element in the zone
    uint size;               // no. of elements in current zone
};


template <typename T>
class FencePointer
{
    std::vector<T> elements;
    std::vector<zone<T>> zones;
    uint num_zones;
    uint num_elements_per_zone;

public:
    // constructor
    fencePointer(std::vector<T> _elements, uint _num_elements_per_zone);

    /**
     * Purpose: builds the zonemap using the elements array/list. Maintains invariant that no. of elements in every zone
     *          is less than or equal to num_elements_per_zone
     * Param: N/A
     * returns: N/A
     */
    void build();

    /**
     * Purpose: sorts the elements in the vector/list/array.
     * Param: N/A
     * returns: N/A
     */
    void sort_elements();

    /**
     * Purpose: Query a key using the zonemap
     * Param: Key to be queried
     * returns: True if element is found, else returns false
     */
    bool query(T key);

    /**
     * Purpose: Query a range of keys using the zonemap that are between a lower and higher limit
     * Param: Lower and upper limit of range to be queried
     * returns: list of elements found using the query
     */
    std::vector<T> query(T low, T high);

    T getGlobalMax();

    T getGlobalMin();
};
#endif