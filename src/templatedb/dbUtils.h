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

LSMTuple::Value turn_db_value_to_lsm_value(const templatedb::Value& val) {
    if (val.visible) {
        LSMTuple::Value newVal = LSMTuple::Value(val.items);
        newVal.visible = val.visible;
        return newVal;
    } else {
        return {val.visible};
    }
}

#endif //TEMPLATEDB_DBUTILS_H
