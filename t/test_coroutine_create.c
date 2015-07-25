#include "test.h"
#include "coroutine.h"
#include <stddef.h>


void func(void *arg) {
  (void) arg;
  return;
}


TEST(coroutine_create) {
  crt_ctx_t *ctx;

  crt_init();

  ctx = crt_create(NULL, func, NULL);
  ASSERT(ctx != NULL, "create crt fail");
  ASSERT(ctx->cid == 2, "cid not match");

  crt_resume(ctx);

  ctx = crt_create(NULL, func, NULL);
  ASSERT(ctx != NULL, "create crt fail");
  ASSERT(ctx->cid == 3, "cid not match");

  crt_resume(ctx);

  ctx = crt_create(NULL, func, NULL);
  ASSERT(ctx != NULL, "create crt fail");
  ASSERT(ctx->cid == 4, "cid not match");

  ctx = crt_create(NULL, func, NULL);
  ASSERT(ctx != NULL, "create crt fail");
  ASSERT(ctx->cid == 5, "cid not match");

  return 0;
}
