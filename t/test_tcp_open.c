#include "test.h"


TEST_IMPL(tcp_open) {
  int ret;
  co_tcp4_t tcp;
  co_ipv4_addr_t addr;

  coroutine_init();

  ret = co_ipv4_addr_init(&addr, "127.0.0.1", 8080);
  ASSERT(ret == 0, str, "co_ipv4_addr_init fail");

  ret = co_tcp4_open(&tcp);
  ASSERT(ret == 0, str, "co_tcp4_open fail");

  ret = co_tcp4_bind(&tcp, &addr);
  ASSERT(ret == 0, str, "co_tcp4_bind fail");

  ret = co_tcp4_listen(&tcp, 1024);
  ASSERT(ret == 0, str, "co_tcp4_listen fail");

  return 0;
}
