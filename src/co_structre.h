#ifndef __CO_STRUCTURE_H__
#define __CO_STRUCTURE_H__


#include <netinet/in.h>


struct co_ipv4_addr_s {
  struct sockaddr_in addr;
  socklen_t          len;
};

struct co_tcp4_s {
  int socket;
};


#endif

