#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>

#include "list.h"
#include "sched.h"
#include "config.h"
#include "context.h"
#include "coroutine.h"


void
coroutine_init()
{
  coroutine_ctx_t *ctx;

  coroutine_sched_init();

  /* init main coroutin */
  ctx = coroutine_ctx_new_main();
  list_add(&ctx->list, &g_coroutine_list);

  g_coroutine_running_ctx = ctx;
}


coroutine_ctx_t*
coroutine_create(const void *attr,
  void*(*start_rtn)(void*), void *arg)
{
  coroutine_ctx_t *ctx;

  /* TODO:
   * corutine attribute.
   */
  (void)attr;

  ctx = coroutine_ctx_new((void(*)())start_rtn, arg);

  /* add to coroutine list */
  list_add(&ctx->list, &g_coroutine_list);
  /* add to ready queue */
  list_add_tail(&ctx->queue, &g_coroutine_ready_list);


#ifdef __DEBUG__
  printf("make coroutine cid: %llu\n", ctx->cid);
#endif

  return ctx;
}


void
coroutine_resume(coroutine_ctx_t *ctx)
{
  coroutine_ctx_t *cur = g_coroutine_running_ctx;

  cur->flag = READY;
  ctx->flag = RUNNING;
  g_coroutine_running_ctx = ctx;

  list_add_tail(&cur->queue, &g_coroutine_ready_list);

  /* if the next coroutine is in ready list, remove it */
  if (!list_is_suspend(&ctx->queue)) {
    list_del(&ctx->queue);
  }

#ifdef __DEBUG__
  printf("resume coroutine cid: %llu\n", ctx->cid);
#endif

  coroutine_sched_swap_context(cur, ctx);
}


void
coroutine_yield()
{
  coroutine_ctx_t *ctx;

  ctx = g_coroutine_running_ctx;

  ctx->flag = READY;
  list_add_tail(&ctx->queue, &g_coroutine_ready_list);

  coroutine_sched();
}


coroutine_ctx_t*
coroutine_running()
{
  return g_coroutine_running_ctx;
}

