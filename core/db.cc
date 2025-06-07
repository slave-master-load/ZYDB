#include "base/skiplist.h"
#include "core/db.h"

namespace zydb {

DB::DB(const DBOptions& optionts)
    : options_(optionts) {
    std::string db_prefix_path = options_.db_dir + "/" + options_.db_name;
    std::string db_path = db_prefix_path + "/db";
    std::string node_path = db_prefix_path + "/node_pool";
    std::string data_path = db_prefix_path + "/data_pool";
    skiplist_ = new base::SkipList(db_path, node_path, data_path);
}

DB::~DB() {
    if (skiplist_) delete skiplist_;
}

void DB::Get(const GetRequest& request, GetResponse* response) {
    const char* ptr = &request.body[0] + request.db_len;
    std::string_view key(ptr, request.key_len);
    auto entry = skiplist_->Find(key);
    if (!entry.Valid()) {
        response->state = static_cast<int8_t>(State::kInvKey);
        response->value_len = Null;
    } else {
        response->state = static_cast<int8_t>(State::kOK);
        auto key = entry.GetKey();
        auto value = entry.GetValue();
        response->key_len = key.length();
        response->value_len = value.length();
        response->key = key.data();
        response->value = value.data();
    }
}

void DB::Put(const PutRequest& request, PutResponse* response) {
    const char* ptr = &request.body[0] + request.db_len;
    std::string_view key(ptr, request.key_len);
    std::string_view value(ptr + request.key_len, request.value_len);
    skiplist_->Remove(key);
    auto put_type = static_cast<PutType>(request.put_type);
    if (put_type != PutType::Delete) skiplist_->Insert(key, value);
    response->state = static_cast<int8_t>(State::kOK);
}

}   // namespace zydb
