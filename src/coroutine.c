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

    coroutine_sched_init();

    coroutine_init_cidmap();
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
    coroutine_ctx_t *mainctx, *ctx;

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
    ctx->cid = cid;
    ctx->flag = READY;
    ctx->stk = (u_char*)mmap(NULL, 8192,
          PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    mainctx = coroutine_get_ctx(0);

    getcontext(&ctx->ctx);
    ctx->ctx.uc_stack.ss_sp = ctx->stk;
    ctx->ctx.uc_stack.ss_size = 8192;
    ctx->ctx.uc_link = &mainctx->ctx;
    makecontext(&ctx->ctx, (void(*)())start_rtn, 1, arg);

    coroutine_set_ctx(cid, ctx);
    printf("make coroutine cid: %d\n", cid);

    *cidp = cid;

    return 0;
}


void
coroutine_resume(coroutine_t cid)
{
    coroutine_t prev = coroutine_self();
    coroutine_ctx_t *prev_ctx = coroutine_get_ctx(prev);
    coroutine_ctx_t *next_ctx = coroutine_get_ctx(cid);

    g_coroutine_running = cid;
    printf("resume coroutine cid: %d\n", cid);
    swapcontext(&prev_ctx->ctx, &next_ctx->ctx);
}


void
coroutine_yield()
{
    int i;
    coroutine_t cid = coroutine_self();

    for (i = 0; i < 1024; ++i) {
        if (cid == i) continue;
        if (coroutine_get_ctx(i) != NULL) {
            coroutine_resume(i);
            break;
        }
    }
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

