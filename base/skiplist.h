#pragma once

#include <memory>
#include <string.h>
#include <string_view>

#include "base/common.h"
#include "base/node_pool.h"
#include "base/data_pool.h"

namespace zydb::base {

class Node;
class DataPool;

struct Entry {
public:
    explicit Entry(Offset off, std::string_view key, std::string_view value)
        : offset(off), key(key), value(value) {}

    Offset GetOffset() const {
        return offset;
    }

    std::string_view GetKey() const {
        return key;
    }

    std::string_view GetValue() const {
        return value;
    }

    bool Valid() const {
        return offset != Null;
    }

    int Compare(std::string_view other) const {
        int min_len = std::min(GetKey().length(), other.length());
        return memcmp(GetKey().data(), other.data(), min_len);
    }

private:
    Offset offset;
    std::string_view key;
    std::string_view value;
};

class SkipList {
    struct Header {
        int32_t head;
        int32_t first_head;
        int32_t entry_num;
    };

public:
    explicit SkipList(std::string_view filename,
                      std::string_view node_pool_path,
                      std::string_view data_pool_path);

    void Insert(std::string_view key, std::string_view value);
    Offset Remove(std::string_view key);

    Entry Find(std::string_view key);

    size_t GetSize() const {
        return header_->entry_num;
    }

private:
    Offset Alloc(std::string_view key, std::string_view value);
    Node* GetNode(int32_t node_id) const {
        return node_pool_->GetNode(node_id);
    }

    Entry GetEntry(Offset offset) const;

    static constexpr int HEIGHT = 17;

    int fd_;
    Header* header_;

    std::unique_ptr<NodePool>  node_pool_;
    std::unique_ptr<DataPool>  data_pool_;

    SkipList(const SkipList&) = delete;
    SkipList& operator=(const SkipList&) = delete;
};

} // namespace zydb::base
