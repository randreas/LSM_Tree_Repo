#include <vector>
#include <string>
#include <math.h>

using namespace std;

class tuple{
public:
	int key;
	Value value;

public:
	tuple(int key, Value value);

	inline void setValue(Value value) {
		this->value = value;
	}
}