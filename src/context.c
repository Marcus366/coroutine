#include <stdlib.h>
#include <assert.h>

#include <sys/mman.h>
#include "sched.h"
#include "context.h"
#include "coroutine.h"


list_head g_crt_list = LIST_HEAD_INIT(g_crt_list);
list_head g_crt_ready_list = LIST_HEAD_INIT(g_crt_ready_list);
list_head g_crt_zombie_list = LIST_HEAD_INIT(g_crt_zombie_list);

crt_ctx_t *g_crt_running_ctx;


void
crt_enter() {
    crt_ctx_t *ctx, *next;

    ctx = g_crt_running_ctx;
    ctx->func(ctx->arg);

    /* running */

    next = crt_sched_find_ready();
    g_crt_running_ctx = next;

    list_del(&next->queue);
    next->flag = RUNNING;

    crt_sched_swap_context(ctx, next);
}


crt_ctx_t*
crt_ctx_new(void(*func)(void*), void *arg)
{
    static unsigned long long uuid = 2;
    crt_ctx_t *ctx;

    ctx = (crt_ctx_t*)malloc(sizeof(crt_ctx_t));
    if (ctx == NULL) {
        return NULL;
    }

    ctx->cid = uuid++;
    ctx->flag = READY;

    ctx->func = func;
    ctx->arg = arg;

    ctx->parent = g_crt_running_ctx;
    ctx->stack  = crt_get_stack();

    return ctx;
}


crt_ctx_t*
crt_ctx_new_main()
{
    crt_ctx_t *ctx;

    ctx = (crt_ctx_t*)malloc(sizeof(crt_ctx_t));
    if (ctx == NULL) {
        return NULL;
    }

    ctx->cid = 1;
    ctx->flag = RUNNING;

    ctx->parent = NULL;
    ctx->stack.base = 0;
    ctx->stack.size = 0;
    ctx->stack.main = 1;

    ctx->list.prev = ctx->list.next = NULL;
    ctx->queue.prev = ctx->queue.next = NULL;

    return ctx;
}


crt_ctx_t*
crt_ctx_new_exit()
{
    /*
    char *sp, *ebp;
    crt_ctx_t *ctx;

    ctx = (crt_ctx_t*)malloc(sizeof(crt_ctx_t));
    if (ctx == NULL) {
        return NULL;
    }

    ctx->flag = READY;

    ctx->parent = g_crt_running_ctx;
    ctx->stack_size = DEFAULT_STACK_SIZE;
    ctx->stack_base = (u_char*)mmap(NULL, ctx->stack_size,
          PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    sp = (char*)ctx->stack_base + ctx->stack_size - 1;
    sp = (char*)((unsigned long)sp & (-16L));


    ebp = sp;
    sp = (char*)sp - 80;
    sp = (char*)((unsigned long)sp & (-16L));

    sp -= 8;
    *(uintptr_t*)sp = (uintptr_t)crt_exit;

    sp -= 8;
    *(uintptr_t*)sp  = (uintptr_t)ebp;
    sp -= 6 * 8;

    ctx->stack_pointer = sp;

    ctx->cid = 0;

    ctx->queue.prev = NULL;
    ctx->queue.next = NULL;
#ifdef __DEBUG__
  printf("make exit crt cid: %llu\n", ctx->cid);
#endif

    return ctx;
    */

    return NULL;
}


void
crt_ctx_free(crt_ctx_t *ctx)
{
    assert(list_is_suspend(&ctx->list));
    assert(list_is_suspend(&ctx->queue));

    crt_put_stack(ctx->stack);
    /* TODO: use pool to manage crt_ctx_t */
    free(ctx);
}

