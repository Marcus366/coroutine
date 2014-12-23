#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>

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
co_tcp4_connect(co_tcp4_t *tcp, co_ipv4_addr_t *addr)
{
  return connect(tcp->socket, (struct sockaddr*)&addr->addr, addr->len);
}


ssize_t
co_tcp4_read(co_tcp4_t *tcp, void *buf, size_t count)
{
  ssize_t n, bytes;

  bytes = 0;
  for (;;) {
    if ((n = read(tcp->socket, ((char*)buf) + bytes, count - bytes)) == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        if (bytes != 0) {
          break;
        }
        coroutine_sched_block(g_coroutine_running_ctx, tcp->socket, EPOLLIN);
        continue;
      } else {
        bytes = -1;
        break;
      }
    }
    bytes += n;

    if (n == 0) {
      break;
    }
  }

  return bytes;
}


ssize_t
co_tcp4_write(co_tcp4_t *tcp, const void *buf, size_t count)
{
  ssize_t n, bytes;

  bytes = 0;
  for (;;) {
    if ((n = write(tcp->socket, ((char*)buf) + bytes, count - bytes)) == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        coroutine_sched_block(g_coroutine_running_ctx, tcp->socket, EPOLLOUT);
        continue;
      } else {
        bytes = -1;
        break;
      }
    }
    bytes += n;

    if (bytes == (ssize_t)count) {
      break;
    }
  }

  return bytes;
}

