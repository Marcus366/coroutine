#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>

#include "list.h"
#include "sched.h"
#include "config.h"
#include "cidmap.h"
#include "context.h"
#include "coroutine.h"


int              g_init;
coroutine_t      g_coroutine_running;


void
coroutine_init()
{
  int i;
  coroutine_t cid;
  coroutine_ctx_t *mainctx;

  g_init = 1;
  for (i = 0; i < 1024; ++i) {
    INIT_HLIST_HEAD(&g_coroutine_map[i]);
  }
  coroutine_cidmap_init();
  coroutine_sched_init();

  cid = coroutine_get_free_cid();
  mainctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
  mainctx->flag = RUNNING;
  getcontext(&mainctx->ctx);
  mainctx->ctx.uc_link = NULL;
  coroutine_set_ctx(cid, mainctx);

  g_coroutine_running = cid;
}


int
coroutine_create(coroutine_t *cidp, const void *attr,
  void*(*start_rtn)(void*), void *arg)
{
  coroutine_t cid;
  coroutine_ctx_t *ctx;

  /* TODO:
   * corutine attribute.
   */
  (void)attr;

  if (!g_init) {
    coroutine_init();
  }

  cid = coroutine_get_free_cid();
  if (cid == -1) {
    return -1;
  }
  *cidp = cid;

  ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
  coroutine_set_ctx(cid, ctx);

  ctx->cid  = cid;
  ctx->flag = READY;
  ctx->stk  = (u_char*)mmap(NULL, 8192000,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);


  getcontext(&ctx->ctx);
  ctx->ctx.uc_stack.ss_sp = ctx->stk;
  ctx->ctx.uc_stack.ss_size = 8192000;
  ctx->ctx.uc_link = &g_exit_coroutine_ctx->ctx;
  makecontext(&ctx->ctx, (void(*)())start_rtn, 1, arg);

  /* add to ready queue */
  list_add_tail(&ctx->queue, &g_coroutine_ready_list);


#ifdef __DEBUG__
  printf("make coroutine cid: %ld\n", cid);
#endif

  return 0;
}


void
coroutine_resume(coroutine_t cid)
{
  coroutine_t cur = coroutine_self();
  coroutine_ctx_t *cur_ctx = coroutine_get_ctx(cur);
  coroutine_ctx_t *next_ctx = coroutine_get_ctx(cid);

  cur_ctx->flag = READY;
  next_ctx->flag = RUNNING;
  g_coroutine_running = cid;

  list_add_tail(&cur_ctx->queue, &g_coroutine_ready_list);

  /* if the next coroutine is in ready list, remove it */
  if (!list_is_suspend(&next_ctx->queue)) {
    list_del(&next_ctx->queue);
  }

#ifdef __DEBUG__
  printf("resume coroutine cid: %ld\n", cid);
#endif

  coroutine_sched_swap_context(cur_ctx, next_ctx);
}


void
coroutine_yield()
{
  coroutine_t cid;
  coroutine_ctx_t *ctx;

  cid = coroutine_self();
  ctx = coroutine_get_ctx(cid);

  ctx->flag = READY;
  list_add_tail(&ctx->queue, &g_coroutine_ready_list);

  coroutine_sched();
}


coroutine_t
coroutine_self()
{
  return g_coroutine_running;
}


int
coroutine_equal(coroutine_t lhs, coroutine_t rhs)
{
  return lhs == rhs;
}

