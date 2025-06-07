#pragma once

#include <string>
#include <vector>
#include <map>

#include "base/common.h"
#include "base/thread_pool.h"
#include "core/message.h"

namespace zydb {

class DB;

struct DBServerOptions {
    std::string db_dir;

    uint16_t port;
    int32_t backlog;

    int32_t thread_num;

    DBServerOptions():
        db_dir("/tmp/db"),
        port(9999),
        backlog(500),
        thread_num(16) {}
};

class DBServer {
public:
    DBServer(const DBServerOptions& options);
    ~DBServer();

    void Start();

private:
    IOBuf ReadData(int client_fd, int length);

    void HandleGetRequest(const GetRequest& request);
    void HandlePutRequest(const PutRequest& request);

    std::map<std::string, DB*> name2db_;

    DBServerOptions options_;

    base::ThreadPool thread_pool_;

    int sockfd_ = Null;
    int client_fd_ = Null;

    DBServer(const DBServer&) = delete;
    DBServer& operator=(const DBServer&) = delete;
};

}   // namespace zydb
