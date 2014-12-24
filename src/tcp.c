#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>

#include "io.h"
#include "sched.h"
#include "co_structre.h"


int
co_ipv4_addr_init(co_ipv4_addr_t *addr, const char *ip, unsigned short port)
{
  memset(addr, 0, sizeof(co_ipv4_addr_t));

  addr->addr.sin_family = AF_INET;

  if (ip == NULL) {
    addr->addr.sin_addr.s_addr = htonl(INADDR_ANY);
  } else if (inet_aton(ip, &addr->addr.sin_addr) == -1) {
    return -1;
  }

  addr->addr.sin_port = htons(port);
  addr->len = sizeof(struct sockaddr_in);

  return 0;
}


int
co_tcp4_open(co_tcp4_t *tcp)
{
  if ((tcp->socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) == -1) {
    return -1;
  }

  return 0;
}


int
co_tcp4_bind(co_tcp4_t *tcp, co_ipv4_addr_t *addr)
{
  return bind(tcp->socket, (struct sockaddr*)&addr->addr, addr->len);
}


int
co_tcp4_listen(co_tcp4_t *tcp, int backlog)
{
  return listen(tcp->socket, backlog);
}


int
co_tcp4_accept(co_tcp4_t *tcp, co_ipv4_addr_t *addr)
{
  int connfd;
  addr->len = sizeof(addr->addr);

loop:
  if ((connfd = accept(tcp->socket, (struct sockaddr*)&addr->addr, &addr->len)) == -1) {
    if (errno == EINTR) {
      goto loop;
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
      coroutine_block(g_coroutine_running_ctx, tcp->socket, EPOLLIN);
      goto loop;
    } else {
      connfd = -1;
    }
  }

  if (connfd != -1) {
    coroutine_register_fd(connfd, 0);
  }

  return connfd;
}


int
co_tcp4_connect(co_tcp4_t *tcp, co_ipv4_addr_t *addr)
{
  return connect(tcp->socket, (struct sockaddr*)&addr->addr, addr->len);
}


ssize_t
co_tcp4_read(co_tcp4_t *tcp, void *buf, size_t count)
{
  return co__read(tcp->socket, buf, count);
}


ssize_t
co_tcp4_write(co_tcp4_t *tcp, const void *buf, size_t count)
{
  return co__write(tcp->socket, buf, count);
}

