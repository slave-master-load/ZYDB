#pragma once

#include <string_view>

#include "base/common.h"

namespace zydb::base {

class DataPool {
private:
    struct Header {
        int64_t file_size;
        int64_t current;
    };

public:
    explicit DataPool(std::string_view filename);

    std::pair<Offset, Pointer> Alloc(int size);
    void Dealloc(Offset offset);

    std::string_view GetData(Offset offset) const;

private:
    void Expand(int32_t size);

    int fd_;
    Header* header_;
    char* data_;

    DataPool(const DataPool&) = delete;
    DataPool& operator=(const DataPool&) = delete;
};

} // namespace zydb::base
