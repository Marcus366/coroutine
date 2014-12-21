#ifndef __TCP_H__
#define __TCP_H__


#include "coroutine.h"
#include <netinet/in.h>


struct co_ipv4_addr_s {
  struct sockaddr_in addr;
  socklen_t          len;
};

struct co_tcp4_s {
  int socket;
};


int co_ipv4_addr_init(co_ipv4_addr_t *addr, const char *ip, unsigned short port);


int co_tcp4_open(co_tcp4_t *tcp);
int co_tcp4_bind(co_tcp4_t *tcp, co_ipv4_addr_t *addr);

int co_tcp4_listen(co_tcp4_t *tcp);

int co_tcp4_connect(co_tcp4_t *tcp, co_ipv4_addr_t *addr);

#endif
