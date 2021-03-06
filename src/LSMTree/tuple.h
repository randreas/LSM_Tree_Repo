#ifndef TEMPLATEDB_TUPLE_H
#define TEMPLATEDB_TUPLE_H

#include <vector>
#include <string>
#include <math.h>
#include <iostream>

#include "const.h"

using namespace std;

namespace LSMTuple {

    class Value {
    public:
        std::vector<int> items;
        bool visible = true;

        Value() {}

        Value(bool del) {
            items = {TERMINATE};
        }

        Value(std::vector<int> items) {
            this->items = items;
        }

        Value(const Value &v) {
            visible = v.visible;
            items.insert(items.end(), v.items.begin(), v.items.end());
        }

        bool IsDelMarker() const {
            if (items.size() > 0) {
                return items[0] == TERMINATE;
            }
            return false;
        }

        bool operator==(const Value &v) {
            if (v.items.size() != items.size()) {
                return false;
            }
            for (int i = 0; i < items.size(); i++) {
                if (items[i] != v.items[i]) {
                    return false;
                }
            }
            return true;
        }

        void printValue() {
            cout << "Value: ";
            if (IsDelMarker()) {
                cout << "deleted";
                return;
            }
            for (int i: items) {
                cout << i << " ";
            }
        }

    };

    class Tuple {
    public:
        int key;
        Value value;

    public:
        Tuple();

        Tuple(int key, Value v);

        inline void setValue(Value v) {
            this->value = v;
        }

        inline Value getValue() const {
            return value;
        }

        inline int getKey() const {
            return key;
        }

        std::string toString() const;

        inline bool isDeleteMarker() const {
            return value.IsDelMarker();
        }

        void printTuple();

    };
}

#endif //TEMPLATEDB_TUPLE_H