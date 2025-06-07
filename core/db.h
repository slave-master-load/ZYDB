#pragma once

#include <string>

#include "core/message.h"

namespace zydb {

namespace base {
class SkipList;
}

struct DBOptions {
    std::string db_dir;
    std::string db_name;

    DBOptions():
        db_dir("/tmp/db"),
        db_name("tmp_db") {}
};

class DB {
public:
    DB(const DBOptions& options);
    ~DB();

    void Get(const GetRequest& request, GetResponse* response);
    void Put(const PutRequest& request, PutResponse* response);

private:
    DBOptions options_;

    base::SkipList* skiplist_;

    DB(const DB&) = delete;
    DB& operator=(const DB&) = delete;
};

}   // namespace zydb
