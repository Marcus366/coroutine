#include "test.h"

#define NR_COROUTINE 2
coroutine_ctx_t *ctx[NR_COROUTINE];
static unsigned long long seq = 0;
static int co1_run = 0;
static int co2_run = 0;

void *func1(void *str) {
  int i;

  for (i = 0; i < 10; ++i) {
    ASSERT(i == seq / 2, "resume fail");
    ++seq;
    ++co1_run;
    coroutine_resume(ctx[1]);
  }

  return NULL;
}

void *func2(void *str) {
  int i;

  for (i = 0; i < 10; ++i) {
    ASSERT(i == seq / 2, "resume fail");
    ++seq;
    ++co2_run;
    coroutine_resume(ctx[0]);
  }

  return NULL;
}


TEST_IMPL(coroutine_resume) {
  coroutine_init();

  ctx[0] = coroutine_create(NULL, func1, str);
  ctx[1] = coroutine_create(NULL, func2, str);

  coroutine_resume(ctx[0]);
  ASSERT(co1_run == 10, "coroutine1 not run enough");
  ASSERT(co2_run == 10, "coroutine2 not run enough");

  return 0;
}
