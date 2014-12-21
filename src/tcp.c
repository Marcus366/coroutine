#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp.h"


int
co_ipv4_addr_init(co_ipv4_addr_t *addr, const char *ip, unsigned short port)
{
  memset(addr, 0, sizeof(co_ipv4_addr_t));

  addr->addr.sin_family = AF_INET;

  if (ip == NULL) {
    addr->addr.sin_addr.s_addr = htonl(INADDR_ANY);
  } if (inet_aton(ip, &addr->addr.sin_addr) == -1) {
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
co_tcp4_listen(co_tcp4_t *tcp)
{
  return listen(tcp->socket, 1024);
}


int
co_tcp4_connect(co_tcp4_t *tcp, co_ipv4_addr_t *addr)
{
  return connect(tcp->socket, (struct sockaddr*)&addr->addr, addr->len);
}

