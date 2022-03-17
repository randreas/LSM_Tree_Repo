#include <vector>
#include <string>
#include <math.h>

#include "const.h"

using namespace std;

class Value {
public:
	std::vector<int> items;
	bool visible = true;

	Value(){}

	Value(std::vector<int> items) {
		this->items = items;
	}

	Value(const Value& v) {
		visible = v.visible;
		items.insert(items.end(), v.items.begin(), v.items.end());
	}

	bool IsDelMarker() const {
        if (items.size() > 0) return items[0] == TERMINATE;
        return false;
    }

}

class Tuple{
public:
	int key;
	Value value;

public:
	tuple(int key, Value v);

	inline void setValue(Value v) {
		this->value = v;
	}

	inline Value getValue() const{
		return value;
	}

	inline int getKey() const{
		return key;
	}

	std::string toString() const;

	inline bool isDeleteMarker() const {
		return value.isDeleteMarker();
	}




}