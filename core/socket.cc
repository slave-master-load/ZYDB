#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "core/socket.h"

namespace zydb {

int NetSocket::Socket() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void NetSocket::Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    if (bind(sockfd, addr, addrlen) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void NetSocket::Listen(int sockfd, int backlog) {
    if (listen(sockfd, backlog) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

int NetSocket::Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen) {
    int client_fd;
    if ((client_fd = accept(sockfd, addr, addrlen)) < 0) {
        perror("Accept failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    return client_fd;
}

void NetSocket::InetPton(int af, const char *src, void *dst) {
    if (inet_pton(af, src, dst) <= 0) {
        perror("InetPton failed");
        exit(EXIT_FAILURE);
    }
}

void NetSocket::Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (connect(sockfd, addr, addrlen) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

} // namespace zydb::net
