#pragma once

#include "base/common.h"

namespace zydb::base {

struct Node {
    Offset offset;
    int32_t next;
    int32_t down;
};

class NodePool {
    struct Header {
        int32_t node_num;
        int32_t first_free;
        int64_t file_size;
    };

public:
    explicit NodePool(std::string_view filename);

    std::pair<NodeId, Node*> Alloc();
    void Dealloc(NodeId node_id);

    Node* GetNode(NodeId node_id);

private:
    void Expand();
    char* GetAddress(NodeId node_id) const {
        return data_ + sizeof(Header) + sizeof(Node) * node_id;
    }

    int fd_;

    Header* header_;
    char* data_;

    NodePool(const NodePool&) = delete;
    NodePool& operator=(const NodePool&) = delete;
};

} // namespace zydb::base
