#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

#include "base/common.h"
#include "core/db_client.h"
#include "core/socket.h"
#include "core/message.h"

namespace zydb {

DBClient::DBClient(const std::string& ip, int port) {
    sockfd_ = NetSocket::Socket();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    NetSocket::InetPton(AF_INET, ip.data(), &addr.sin_addr);

    NetSocket::Connect(sockfd_, (struct sockaddr*)&addr, sizeof(addr));
}

void DBClient::Get(const GetRequest& request, GetResponse* response) {
    MessageHead head;
    head.length = sizeof(int32_t) * 2 + request.db_len + request.key_len;
    head.method = static_cast<int8_t>(Method::GET);
    write(sockfd_, (const void*)&head, sizeof(head));
    write(sockfd_, (const void*)&request, head.length);

    read(sockfd_, &response->state, sizeof(response->state));
    read(sockfd_, &response->key_len, sizeof(response->key_len));
    read(sockfd_, &response->value_len, sizeof(response->value_len));

    if (static_cast<State>(response->state) == State::kOK) {
        char* tmp_key = (char*)malloc(sizeof(char) * response->key_len);
        char* tmp_value = (char*)malloc(sizeof(char) * response->value_len);
        read(sockfd_, tmp_key, response->key_len);
        read(sockfd_, tmp_value, response->value_len);
        response->key = tmp_key;
        response->value = tmp_value;
    }
}

void DBClient::Put(const PutRequest& request, PutResponse* response) {
    MessageHead head;
    head.length = sizeof(int32_t) * 3 +
            sizeof(int8_t) + request.db_len + request.key_len + request.value_len;
    head.method = static_cast<int8_t>(Method::PUT);
    write(sockfd_, (const void*)&head, sizeof(head));
    write(sockfd_, (const void*)&request, head.length);

    IOBuf buffer(sizeof(PutResponse));
    read(sockfd_, buffer.data(), sizeof(PutResponse));
    auto& resp = *reinterpret_cast<PutResponse*>(buffer.data());
    response->state = resp.state;
}

DBClient::~DBClient() {
    if (sockfd_ != Null) close(sockfd_);
}

}   // namespace zydb
