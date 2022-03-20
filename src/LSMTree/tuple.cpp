
#include "tuple.h"
#include "const.h"
#include <stdlib.h>
#include <string.h>
#include <utility>

using namespace std;

Tuple::Tuple(int key, Value val):
    key(key), value(val){
}


Tuple::Tuple() {
	value = Value();
}

//ToString
//Print

