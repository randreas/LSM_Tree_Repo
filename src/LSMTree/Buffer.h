#include "tuple.h"
#include "FileMeta.h"

class Buffer{
private:
	std::vector <LSMTuple::Tuple> tuples;
	int max_size;

public:
	Buffer(int size);
	bool isEmpty();
	std::vector<LSMTuple::Tuple> getTuples();

	bool Append(int key, LSMTuple::Value v);
	bool isFull();

    FileMeta* merge();

	//bool Scan(userAskedRange, searchRange, ) 
};