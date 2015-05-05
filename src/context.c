#include <stdlib.h>
#include <assert.h>
#include <sys/mman.h>
#include "sched.h"
#include "context.h"
#include "coroutine.h"

#define DEFAULT_STACK_SIZE (8192 * 1024)


list_head g_coroutine_list = LIST_HEAD_INIT(g_coroutine_list);
list_head g_coroutine_ready_list = LIST_HEAD_INIT(g_coroutine_ready_list);

coroutine_ctx_t *g_coroutine_running_ctx;


void coroutine_exit_swap() {
  coroutine_resume(g_exit_coroutine_ctx);
}


coroutine_ctx_t*
coroutine_ctx_new(void(*func)(), void *arg)
{
  static unsigned long long uuid = 2;
  char *sp, *ebp;
  coroutine_ctx_t *ctx;

  ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
  if (ctx == NULL) {
    return NULL;
  }

  ctx->cid = uuid++;
  ctx->flag = READY;

  ctx->parent = g_coroutine_running_ctx;
  ctx->stack_size = DEFAULT_STACK_SIZE;
  ctx->stack_base = (u_char*)mmap(NULL, ctx->stack_size,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

  sp = (char*)ctx->stack_base + ctx->stack_size - 1;
  sp = (char*)((unsigned long)sp & (-16L));

  sp -= 8;
  *(uintptr_t*)sp = (uintptr_t)0;
  sp -= 8;
  *(uintptr_t*)sp = (uintptr_t)coroutine_resume;
  sp -= 16;

  ebp = sp;
  sp = (char*)sp - 80;
  sp = (char*)((unsigned long)sp & (-16L));

  sp -= 8;
  *(uintptr_t*)sp = (uintptr_t)coroutine_exit_swap;
  sp -= 8;
  *(uintptr_t*)sp = (uintptr_t)func;

  sp -= 8;
  *(uintptr_t*)sp  = (uintptr_t)ebp;
  sp -= 6 * 8;

  ctx->stack_pointer = sp;

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

  ctx->parent = NULL;
  ctx->stack_base = 0;

  ctx->list.prev = ctx->list.next = NULL;
  ctx->queue.prev = ctx->queue.next = NULL;

  return ctx;
}


coroutine_ctx_t*
coroutine_ctx_new_exit()
{
  char *sp, *ebp;
  coroutine_ctx_t *ctx;

  ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
  if (ctx == NULL) {
    return NULL;
  }

  ctx->flag = READY;

  ctx->parent = g_coroutine_running_ctx;
  ctx->stack_size = DEFAULT_STACK_SIZE;
  ctx->stack_base = (u_char*)mmap(NULL, ctx->stack_size,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  
  sp = (char*)ctx->stack_base + ctx->stack_size - 1;
  sp = (char*)((unsigned long)sp & (-16L));


  ebp = sp;
  sp = (char*)sp - 80;
  sp = (char*)((unsigned long)sp & (-16L));

  sp -= 8;
  *(uintptr_t*)sp = (uintptr_t)coroutine_exit;

  sp -= 8;
  *(uintptr_t*)sp  = (uintptr_t)ebp;
  sp -= 6 * 8;

  ctx->stack_pointer = sp;
  /*
  ctx->stack_pointer -= 16;

  *(uintptr_t*)ctx->stack_pointer = (uintptr_t)0;
  ctx->stack_pointer--;

  *(uintptr_t*)ctx->stack_pointer = (uintptr_t)coroutine_exit;
  ctx->stack_pointer--;

  ctx->stack_pointer -= 5;

  *(uintptr_t*)ctx->stack_pointer = (uintptr_t)ctx->stack_pointer + 6;
  ctx->stack_pointer--;
  */

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

  munmap(ctx->stack_base, ctx->stack_size);
  free(ctx);
}

