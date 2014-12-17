#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>

#include "list.h"
#include "sched.h"
#include "config.h"
#include "context.h"
#include "coroutine.h"


int              g_init;


void
coroutine_init()
{
  coroutine_ctx_t *ctx;

  g_init = 1;
  coroutine_sched_init();

  ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
  ctx->flag = RUNNING;
  getcontext(&ctx->ctx);
  ctx->parent = NULL;
  ctx->ctx.uc_link = &g_exit_coroutine_ctx->ctx;
  ctx->queue.prev = NULL;
  ctx->queue.next = NULL;

#ifdef __DEBUG__
  ctx->cid = 6666666;
#endif

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

  if (!g_init) {
    coroutine_init();
  }

  ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
#ifdef __DEBUG__
  static unsigned long long uuid = 0;
  ctx->cid = uuid++;
#endif

  ctx->flag = READY;
  getcontext(&ctx->ctx);
  ctx->parent = g_coroutine_running_ctx;
  ctx->ctx.uc_stack.ss_sp = (u_char*)mmap(NULL, 8192000,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  ctx->ctx.uc_stack.ss_size = 8192000;
  //ctx->ctx.uc_link = &coroutine_get_ctx(g_coroutine_running)->ctx;
  ctx->ctx.uc_link = &g_exit_coroutine_ctx->ctx;
  makecontext(&ctx->ctx, (void(*)())start_rtn, 1, arg);

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

