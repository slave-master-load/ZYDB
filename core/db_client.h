#pragma once

#include <string>

#include "base/common.h"
#include "core/message.h"

namespace zydb {

class DBClient {
public:
    DBClient(const std::string& ip, int port);
    ~DBClient();

    void Get(const GetRequest& request, GetResponse* response);
    void Put(const PutRequest& request, PutResponse* response);

private:
    int sockfd_ = Null;

    DBClient(const DBClient&) = delete;
    DBClient& operator=(const DBClient&) = delete;
};

}   // namespace zydb
