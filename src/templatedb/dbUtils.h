//
// Created by jingyusu on 4/14/22.
//

#ifndef TEMPLATEDB_DBUTILS_H
#define TEMPLATEDB_DBUTILS_H

#include "db.hpp"
#include "../tuple"

templatedb::Value turn_tree_value_to_db_value(const LSMTuple::Value& val) {
    templatedb::Value newVal = templatedb::Value(val.items);
    newVal.visible = val.IsDelMarker();
    return newVal;
}

#endif //TEMPLATEDB_DBUTILS_H
