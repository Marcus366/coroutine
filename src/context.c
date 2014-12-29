#define __USE_GNU
#include <sys/ucontext.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include "sched.h"
#include "context.h"
#include "coroutine.h"



list_head  g_coroutine_list = LIST_HEAD_INIT(g_coroutine_list);
list_head  g_coroutine_ready_list = LIST_HEAD_INIT(g_coroutine_ready_list);

coroutine_ctx_t *g_coroutine_running_ctx;


coroutine_ctx_t*
coroutine_ctx_new(void(*func)(), void *arg)
{
  static unsigned long long uuid = 2;
  coroutine_ctx_t *ctx;

  ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
  if (ctx == NULL) {
    return NULL;
  }

  ctx->cid = uuid++;
  ctx->flag = READY;

  getcontext(&ctx->ctx);
  ctx->parent = g_coroutine_running_ctx;
  ctx->ctx.uc_stack.ss_sp = (u_char*)mmap(NULL, 8192000,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  ctx->ctx.uc_stack.ss_size = 8192000;
  ctx->ctx.uc_link = &g_exit_coroutine_ctx->ctx;
  ctx->ctx.uc_mcontext.gregs[15] -= 6;
  makecontext(&ctx->ctx, func, 1, arg);

  return ctx;
}


coroutine_ctx_t*
coroutine_ctx_new_main()
{
  coroutine_ctx_t *ctx;

  ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
  if (ctx == NULL) {
    return NULL;
  }

  ctx->cid = 1;
  ctx->flag = RUNNING;

  getcontext(&ctx->ctx);
  ctx->parent = NULL;
  ctx->ctx.uc_link= &g_exit_coroutine_ctx->ctx;

  ctx->list.prev = NULL;
  ctx->list.next = NULL;

  ctx->queue.prev = NULL;
  ctx->queue.next = NULL;

  return ctx;
}


coroutine_ctx_t*
coroutine_ctx_new_exit()
{
  coroutine_ctx_t *ctx;

  ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
  if (ctx == NULL) {
    return NULL;
  }

  ctx->flag = READY;

  getcontext(&ctx->ctx);
  ctx->ctx.uc_stack.ss_sp = mmap(NULL, 8192000,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  ctx->ctx.uc_stack.ss_size = 8192000;
  ctx->ctx.uc_link = NULL;
  makecontext(&ctx->ctx, (void(*)())coroutine_exit, 1, NULL);

  ctx->cid = 0;

  ctx->queue.prev = NULL;
  ctx->queue.next = NULL;
#ifdef __DEBUG__
  printf("make exit coroutine cid: %llu\n", ctx->cid);
#endif

  return ctx;
}


void
coroutine_ctx_free(coroutine_ctx_t *ctx)
{
  assert(list_is_suspend(&ctx->list));
  assert(list_is_suspend(&ctx->queue));

  munmap(ctx->ctx.uc_stack.ss_sp, ctx->ctx.uc_stack.ss_size);
  free(ctx);
}

