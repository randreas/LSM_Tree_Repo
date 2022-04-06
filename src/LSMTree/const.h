/*
File to store Constants 
*/
#include <limits.h>

#define TERMINATE	INT_MIN
#define BF_NUM_TUPLES 10000
#define BF_BITS_PER_ELEMENT 8

#define SIZEOFINT 4
#define MAX_TUPLE_KEYS_CNTS    1 /// The number of keys per tuple
#define MAX_TUPLE_VALUES_CNTS   2 /// The number of values per tuple


inline int getBFNumTuples() {
    return BF_NUM_TUPLES;
}

inline int getBFBitsPerElement() {
    return BF_BITS_PER_ELEMENT;
}

inline int getTupleBytesSize() {
	return (MAX_TUPLE_KEYS_CNTS + MAX_TUPLE_VALUES_CNTS) * SIZEOFINT;
}
