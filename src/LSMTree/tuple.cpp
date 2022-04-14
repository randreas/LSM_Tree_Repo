
#include "tuple.h"
#include "const.h"
#include <stdlib.h>
#include <string.h>
#include <utility>

using namespace std;

LSMTuple::Tuple::Tuple(int key, Value val):
    key(key), value(val){
}


LSMTuple::Tuple::Tuple() {
	value = Value();
}

//ToString

//Print

void LSMTuple::Tuple::printTuple() {
    cout << "| Key: " << key << " ";
    value.printValue();
    cout << "\n";
}

