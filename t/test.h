#ifndef __TEST_H__
#define __TEST_H__


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../src/coroutine.h"


#define TEST_DECLARE(name)      \
  int test_##name(char *str)

#define TEST_IMPL(name)         \
  int test_##name(char *str);   \
  int test_##name(char *str)

#define ASSERT(assert, log)      \
  if (!(assert)) {               \
    sprintf(str, "%s", log);     \
    exit(-1);                    \
  }

typedef struct test_entry_s {
  int  (*func)(char *str);

  char*  name;
  int    code;
  char   log[64];
} test_entry_t;


#endif

