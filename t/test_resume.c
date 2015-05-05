#include <stdio.h>
#include <stdlib.h>
#include "../src/coroutine.h"

#define NR_COROUTINE 2
coroutine_ctx_t *ctx[NR_COROUTINE];
static unsigned long long seq = 0;
static int co1_run = 0;
static int co2_run = 0;

void func1(void *str) {
  int i;
  (void) str;

  /*`
  for (i = 0; i < 10; ++i) {
    ++seq, ++co1_run;
    coroutine_resume(ctx[1]);
  }

  */
  return;
}

void func2(void *str) {
  int i;
  (void) str;

  for (i = 0; i < 10; ++i) {
    ++seq, ++co2_run;
    coroutine_resume(ctx[0]);
  }

  return;
}


int main() {
  coroutine_init();

  ctx[0] = coroutine_create(NULL, func1, 10);
  //ctx[1] = coroutine_create(NULL, func2, NULL);

  coroutine_resume(ctx[0]);

  return 0;
}
