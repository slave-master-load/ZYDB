#include <sys/mman.h>
#include <string.h>
#include <cassert>

#include "base/data_pool.h"
#include "base/file_util.h"

namespace zydb::base {

DataPool::DataPool(std::string_view filename) {
    bool need_init = false;
    if (!FileUtil::Exists(filename)) {
        need_init = true;
        FileUtil::CreateFile(filename);
    }

    fd_ = open(filename.data(), O_RDWR);
    CHECK(fd_ != -1);

    fallocate(fd_, 0, 0, sizeof(Header));
    data_ = (char*)mmap(nullptr, MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    CHECK(data_ != MAP_FAILED);

    header_ = reinterpret_cast<Header*>(data_);
    if (need_init) {
        header_->file_size = sizeof(Header);
        header_->current = sizeof(Header);
    }
}

std::pair<Offset, Pointer> DataPool::Alloc(int size) {
    Expand(size + 4);
    Offset offset = header_->current;
    char* ptr = data_ + offset;
    *reinterpret_cast<int32_t*>(ptr) = size;
    header_->current += size + 4;
    return {offset, ptr + 4};
}

void DataPool::Dealloc(Offset offset) {
    //TODO: dealloc offset
}

std::string_view DataPool::GetData(Offset offset) const {
    if (offset == Null) return std::string_view(nullptr, 0);
    const char* ptr = data_ + offset;
    auto length = *reinterpret_cast<const int32_t*>(ptr);
    return std::string_view(ptr + 4, length);
}

void DataPool::Expand(int32_t size) {
    fallocate(fd_, 0, header_->file_size, size);
    header_->file_size += size;
}

} // namespace zydb::base
