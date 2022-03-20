#include "Tuple.h"


class Buffer{
private:
	std::vector <Tuple> tuples;
	int max_size;

public:
	Buffer(int size);
	bool isEmpty();
	std::vector<Tuple> getTuples();

	bool Append(int key, Value v);
	bool isFull();

	//bool Scan(userAskedRange, searchRange, ) 
}