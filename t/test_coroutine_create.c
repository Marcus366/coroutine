#include "test.h"


void func(void *arg) {
  (void) arg;
  return;
}


TEST_IMPL(coroutine_create) {
  coroutine_ctx_t *ctx;

  coroutine_init();

  ctx = coroutine_create(NULL, func, NULL);
  ASSERT(ctx != NULL, "create coroutine fail");
  ASSERT(ctx->cid == 1, "cid not match");

  coroutine_resume(ctx);

  ctx = coroutine_create(NULL, func, NULL);
  ASSERT(ctx != NULL, "create coroutine fail");
  ASSERT(ctx->cid == 1, "cid not match");

  coroutine_resume(ctx);

  ctx = coroutine_create(NULL, func, NULL);
  ASSERT(ctx != NULL, "create coroutine fail");
  ASSERT(ctx->cid == 1, "cid not match");

  ctx = coroutine_create(NULL, func, NULL);
  ASSERT(ctx != NULL, "create coroutine fail");
  ASSERT(ctx->cid == 2, "cid not match");

  return 0;
}
