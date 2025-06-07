#pragma once

namespace zydb {

class NetSocket {
public:
    static int Socket();

    static void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);

    static void Listen(int sockfd, int backlog);

    static int Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlenn);

    static void InetPton(int af, const char *src, void *dst);

    static void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
};

} // namespace zydb
