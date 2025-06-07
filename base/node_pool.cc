#include <sys/mman.h>
#include <cassert>
#include <string_view>

#include "base/node_pool.h"
#include "base/file_util.h"

namespace zydb::base {

NodePool::NodePool(std::string_view filename) {
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
        header_->node_num = 0;
        header_->first_free = Null;
        header_->file_size = sizeof(Header);
    }
}

std::pair<NodeId, Node*> NodePool::Alloc() {
    if (header_->first_free == Null) Expand();

    auto node_id = header_->first_free;
    auto next_id = *reinterpret_cast<const int32_t*>(GetAddress(node_id));
    header_->first_free = next_id;
    return {node_id, GetNode(node_id)};
}

void NodePool::Dealloc(NodeId node_id) {
    *reinterpret_cast<int32_t*>(GetAddress(node_id)) = header_->first_free;
    header_->first_free = node_id;
}

Node* NodePool::GetNode(NodeId node_id) {
    if (node_id >= header_->node_num) return nullptr;
    return reinterpret_cast<Node*>(GetAddress(node_id));
}

void NodePool::Expand() {
    fallocate(fd_, 0, header_->file_size, sizeof(Node));
    header_->file_size += sizeof(Node);
    header_->first_free = header_->node_num;
    *reinterpret_cast<int32_t*>(GetAddress(header_->node_num)) = -1;
    header_->node_num += 1;
}

} // namespace zydb::base
