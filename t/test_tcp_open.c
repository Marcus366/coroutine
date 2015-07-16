#include "test.h"


TEST_IMPL(tcp_open) {
  int ret;
  co_tcp_t tcp;
  struct sockaddr_in addr;

  crt_init();

  ret = co_ip4_addr_init(&addr, "127.0.0.1", 8080);
  ASSERT(ret == 0, "co_ipv4_addr_init fail");

  ret = co_tcp4_open(&tcp);
  ASSERT(ret == 0, "co_tcp4_open fail");

  ret = co_tcp_bind(&tcp, (struct sockaddr*)&addr);
  ASSERT(ret == 0, "co_tcp4_bind fail");

  ret = co_tcp_listen(&tcp, 1024);
  ASSERT(ret == 0, "co_tcp4_listen fail");

  return 0;
}
