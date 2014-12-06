#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>

#include "list.h"
#include "sched.h"
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
    coroutine_ctx_t *main_ctx;

    g_init = 1;
    for (i = 0; i < 1024; ++i) {
        INIT_HLIST_HEAD(&g_coroutine_map[i]);
    }

    coroutine_cidmap_init();
    coroutine_sched_init();

    cid = coroutine_get_free_cid();
    main_ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
    main_ctx->flag = RUNNING;
    getcontext(&main_ctx->ctx);
    coroutine_set_ctx(cid, main_ctx);
}


int
coroutine_create(coroutine_t *cidp, const void *attr,
    void*(*start_rtn)(void*), void *arg)
{
    coroutine_t cid;
    coroutine_ctx_t *exitctx, *ctx;

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

    ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
    coroutine_set_ctx(cid, ctx);

    ctx->cid  = cid;
    ctx->flag = READY;
    ctx->stk  = (u_char*)mmap(NULL, 8192,
          PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    exitctx = coroutine_get_ctx(g_exit_coroutine);

    getcontext(&ctx->ctx);
    ctx->ctx.uc_stack.ss_sp = ctx->stk;
    ctx->ctx.uc_stack.ss_size = 8192;
    ctx->ctx.uc_link = &exitctx->ctx;
    makecontext(&ctx->ctx, (void(*)())start_rtn, 1, arg);

    printf("make coroutine cid: %ld\n", cid);

    *cidp = cid;

    return 0;
}


void
coroutine_resume(coroutine_t cid)
{
    coroutine_t prev = coroutine_self();
    coroutine_ctx_t *prev_ctx = coroutine_get_ctx(prev);
    coroutine_ctx_t *next_ctx = coroutine_get_ctx(cid);

    prev_ctx->flag = READY;
    next_ctx->flag = RUNNING;
    g_coroutine_running = cid;
    printf("resume coroutine cid: %ld\n", cid);
    swapcontext(&prev_ctx->ctx, &next_ctx->ctx);
}


void
coroutine_yield()
{
    coroutine_t cid;
    
    cid = coroutine_sched(SCHED_NOWAIT);
    if (cid == -1) {
        cid = coroutine_sched(SCHED_BLOCK);
    }

    coroutine_resume(cid);
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

