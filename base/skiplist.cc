#include <sys/mman.h>
#include <cassert>

#include "base/skiplist.h"
#include "base/file_util.h"

namespace zydb::base {

SkipList::SkipList(std::string_view filename,
                   std::string_view node_pool_path,
                   std::string_view data_pool_path) {
    bool need_init = false;
    if (!FileUtil::Exists(filename)) {
        need_init = true;
        FileUtil::CreateFile(filename);
    }

    fd_ = open(filename.data(), O_RDWR);
    CHECK(fd_ != -1);

    fallocate(fd_, 0, 0, sizeof(Header));
    header_ = (Header*)mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    CHECK(header_ != MAP_FAILED);

    node_pool_ = std::make_unique<NodePool>(node_pool_path);
    data_pool_ = std::make_unique<DataPool>(data_pool_path);

    if (need_init) {
        int32_t last_id = Null;
        for (int i = 0; i < HEIGHT; ++i) {
            auto [head_id, head_node] = node_pool_->Alloc();
            if (i == 0) header_->first_head = head_id;
            head_node->offset = Null;
            head_node->next = Null;
            head_node->down = last_id;
            last_id = head_id;
        }
        header_->head = last_id;
        header_->entry_num = 0;
    }
}

void SkipList::Insert(std::string_view key, std::string_view value) {
    auto offset = Alloc(key, value);
    auto node_id = header_->head;
    std::vector<Node*> path;
    while (node_id != Null) {
        auto node = GetNode(node_id);
        if (node->next == Null) {
            path.push_back(node);
            node_id = node->down;
        } else {
            auto next_node = GetNode(node->next);
            auto next_entry = GetEntry(next_node->offset);
            int cmp_ret = next_entry.Compare(key);
            if (cmp_ret == 0) return;
            if (cmp_ret < 0) {
                node_id = node->next;
            } else {
                path.push_back(node);
                node_id = node->down;
            }
        }
    }

    Random random;
    int32_t last_id = Null;
    for (size_t i = path.size() - 1; i >= 0; --i) {
        auto node = path[i];
        auto [new_node_id, new_node] = node_pool_->Alloc();
        new_node->next = node->next;
        new_node->down = last_id;
        new_node->offset = offset;
        node->next = new_node_id;
        last_id = new_node_id;
        if (random.GetInt(1, 10) < 5) break;
    }
    header_->entry_num++;
}

Offset SkipList::Remove(std::string_view key) {
    auto node_id = header_->head;
    Offset offset = Null;
    while (node_id != Null) {
        auto node = GetNode(node_id);
        if (node->next == Null) {
            node_id = node->down;
        } else {
            auto next_node = GetNode(node->next);
            auto next_entry = GetEntry(next_node->offset);
            auto cmp_ret = next_entry.Compare(key);
            if (cmp_ret < 0) {
                node_id = node->next;
                continue;
            }
            if (cmp_ret == 0) {
                node_pool_->Dealloc(node->next);
                node->next = next_node->next;
                offset = next_node->offset;
            }
            node_id = node->down;
        }
    }
    if (offset != Null) header_->entry_num--;
    return offset;
}

Entry SkipList::Find(std::string_view key) {
    auto node_id = header_->head;
    Offset offset = Null;
    while (node_id != Null) {
        auto node = GetNode(node_id);
        if (node->next == Null) {
            node_id = node->down;
        } else {
            auto next_node = GetNode(node->next);
            auto next_entry = GetEntry(next_node->offset);
            auto cmp_ret = next_entry.Compare(key);
            if (cmp_ret < 0) {
                node_id = node->next;
                continue;
            }
            if (cmp_ret == 0) return next_entry;
            node_id = node->down;
        }
    }
    return Entry(Null, {}, {});
}

Offset SkipList::Alloc(std::string_view key, std::string_view value) {
    int size = 8 + key.length() + value.length();
    auto [offset, ptr] = data_pool_->Alloc(size);
    *reinterpret_cast<int32_t*>(ptr) = key.length();
    *reinterpret_cast<int32_t*>(ptr + 4) = value.length();
    memcpy(ptr + 8, key.data(), key.length());
    memcpy(ptr + 8 + key.length(), value.data(), value.length());
    return offset;
}

Entry SkipList::GetEntry(Offset offset) const {
    if (offset == Null) return Entry(offset, {}, {});
    auto raw_entry = data_pool_->GetData(offset);
    const char* ptr = raw_entry.data();
    int32_t key_len = *reinterpret_cast<const int32_t*>(ptr);
    int32_t value_len = *reinterpret_cast<const int32_t*>(ptr + 4);
    std::string_view key(ptr + 8, key_len);
    std::string_view value(ptr + 8 + key_len, value_len);
    return Entry(offset, key, value);
}

} // namespace zydb::base
