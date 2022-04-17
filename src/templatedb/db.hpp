#ifndef _TEMPLATEDB_DB_H_
#define _TEMPLATEDB_DB_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "../LSMTree/LSMTree.h"

#include "templatedb/operation.hpp"

namespace templatedb
{

typedef enum _status_code
{
    OPEN = 0,
    CLOSED = 1,
    ERROR_OPEN = 100,
} db_status;


class Value
{
public:
    std::vector<int> items;
    bool visible = true;

    Value() {}
    Value(bool _visible) {visible = _visible;}
    Value(std::vector<int> _items) { items = _items;}

    bool operator ==(Value const & other) const
    {
        return (visible == other.visible) && (items == other.items);
    }
};


class DB
{
private:
    LSMTree* lsm_tree_idx;

public:
    db_status status;

    DB() {};

    DB(int _initial_run_size, int _num_run_per_level) {
        this->lsm_tree_idx = new LSMTree(_initial_run_size, _num_run_per_level, true);
        this->status = OPEN;
    };

    //TODO: Clear index
    ~DB() {close();};

    //TODO: USE LSMTree in these ops
    Value get(int key);
    void put(int key, Value val);
    std::vector<Value> scan();
    std::vector<Value> scan(int min_key, int max_key);
    void del(int key);

    //TODO: IMPLEMENT THIS
    void del(int min_key, int max_key);
    size_t size();

    db_status open(std::string & fname);
    bool close();

    bool load_data_file(std::string & fname);

    std::vector<Value> execute_op(Operation op);

private:
    std::fstream file;
    std::unordered_map<int, Value> table;
    size_t value_dimensions = 0;
    
    bool write_to_file();
};

}   // namespace templatedb

#endif /* _TEMPLATEDB_DB_H_ */