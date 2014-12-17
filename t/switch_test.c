#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/coroutine.h"

#define COROUTINE_SIZE 10000

coroutine_ctx_t *c[COROUTINE_SIZE];


void* foo(void *arg) {
  int i;
  (void) arg;
  for (i = 0; i < 10; ++i) {
    coroutine_yield();
  }

  return NULL;
}


int main()
{
  long i;
  for (i = 0; i < COROUTINE_SIZE; ++i) {
    coroutine_create(NULL, foo, (void*)i);
  }
  coroutine_resume(c[0]);

  return 0;
}
