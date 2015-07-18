#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>

#include "sched.h"
#include "utils.h"


int
co_ip4_addr_init(struct sockaddr_in *addr, const char *ip, unsigned short port)
{
    memset(addr, 0, sizeof(struct sockaddr_in));

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);

    if (ip != NULL) {
        return inet_pton(AF_INET, ip, &addr->sin_addr);
    }

    addr->sin_addr.s_addr = htonl(INADDR_ANY);
    return 0;
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
co_tcp4_open()
{
    int fd;

    if ((fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
        return -1;
    }

    if (set_nonblocking(fd)) {
        close(fd);
        fd = -1;
    }

    return fd;
}


int
co_tcp6_open()
{
    int fd;

    if ((fd = socket(AF_INET6, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
        return -1;
    }

    return fd;
}


int
co_tcp4_open_bind(const char *ip, int port)
{
    int fd;
    struct sockaddr_in addr;

    fd = co_tcp4_open();
    if (fd == -1) {
        return -1;
    }

    if (co_ip4_addr_init(&addr, ip, port)) {
        return -1;
    }

    if (co_tcp_bind(fd, (struct sockaddr*)&addr)) {
        return -1;
    }

    return fd;
}


int
co_tcp6_open_bind(const char *ip, int port)
{
    /* TODO */
    return -1;
}


int
co_tcp_bind(int fd, const struct sockaddr *addr)
{
    if (addr->sa_family == AF_INET) {
        return bind(fd, addr, sizeof(struct sockaddr_in));
    } else if (addr->sa_family == AF_INET6) {
        return bind(fd, addr, sizeof(struct sockaddr_in6));
    }

    return -1;
}


int
co_tcp_listen(int fd, int backlog)
{
    return listen(fd, backlog);
}


int
co_tcp_accept(int fd, struct sockaddr *addr, socklen_t *len)
{
    int connfd;

loop:
    if ((connfd = accept(fd, addr, len)) == -1) {
        if (errno == EINTR) {
            goto loop;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            crt_block(fd, EPOLLIN);
            goto loop;
        } else {
            connfd = -1;
        }
    }

    if (connfd != -1) {
        if (set_nonblocking(connfd)) {
            close(connfd);
            return -1;
        }
        //crt_register_fd(connfd);
    }

    return connfd;
}


int
co_tcp_connect(int fd, struct sockaddr *addr)
{
    if (addr->sa_family == AF_INET) {
        return connect(fd, addr, sizeof(struct sockaddr_in));
    } else if (addr->sa_family == AF_INET6) {
        return connect(fd, addr, sizeof(struct sockaddr_in6));
    }

    return -1;
}


ssize_t
co_tcp_read(int fd, void *buf, size_t count)
{
    return co_read(fd, buf, count);
}


ssize_t
co_tcp_write(int fd, const void *buf, size_t count)
{
    return co_write(fd, buf, count);
}

