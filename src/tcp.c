#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>

#include "sched.h"


int
co_ip4_addr_init(struct sockaddr_in *addr, const char *ip, unsigned short port)
{
    memset(addr, 0, sizeof(struct sockaddr_in));

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);

    return inet_pton(AF_INET, ip, &addr->sin_addr);
}


int
co_ip6_addr_init(struct sockaddr_in6 *addr, const char *ip, unsigned short port)
{
    memset(addr, 0, sizeof(struct sockaddr_in6));

    addr->sin6_family = AF_INET6;
    addr->sin6_port = htons(port);

    return inet_pton(AF_INET, ip, &addr->sin6_addr);
}


int
co_tcp4_open(co_tcp_t *tcp)
{
    if ((tcp->socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
        return -1;
    }

    return 0;
}


int
co_tcp6_open(co_tcp_t *tcp)
{
    if ((tcp->socket = socket(AF_INET6, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
        return -1;
    }

    return 0;
}


int
co_tcp_bind(co_tcp_t *tcp, const struct sockaddr *addr)
{
    if (addr->sa_family == AF_INET) {
        return bind(tcp->socket, addr, sizeof(struct sockaddr_in));
    } else if (addr->sa_family == AF_INET6) {
        return bind(tcp->socket, addr, sizeof(struct sockaddr_in6));
    }

    return -1;
}


int
co_tcp_listen(co_tcp_t *tcp, int backlog)
{
    return listen(tcp->socket, backlog);
}


int
co_tcp_accept(co_tcp_t *tcp, struct sockaddr *addr, socklen_t *len)
{
    int connfd;

loop:
    if ((connfd = accept(tcp->socket, addr, len)) == -1) {
        if (errno == EINTR) {
            goto loop;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            crt_block(tcp->socket, EPOLLIN);
            goto loop;
        } else {
            connfd = -1;
        }
    }

    if (connfd != -1) {
        crt_register_fd(connfd);
    }

    return connfd;
}


int
co_tcp_connect(co_tcp_t *tcp, struct sockaddr *addr)
{
    if (addr->sa_family == AF_INET) {
        return connect(tcp->socket, addr, sizeof(struct sockaddr_in));
    } else if (addr->sa_family == AF_INET6) {
        return connect(tcp->socket, addr, sizeof(struct sockaddr_in6));
    }

    return -1;
}


ssize_t
co_tcp_read(co_tcp_t *tcp, void *buf, size_t count)
{
    return co_read(tcp->socket, buf, count);
}


ssize_t
co_tcp_write(co_tcp_t *tcp, const void *buf, size_t count)
{
    return co_write(tcp->socket, buf, count);
}

