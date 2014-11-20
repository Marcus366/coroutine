#include "coroutine.h"
#include "cidmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <stdint.h>
#include <sys/mman.h>


typedef struct {
    u_char    *stk;
    ucontext_t ctx;
} coroutine_ctx_t;


coroutine_t      g_coroutine_running;
coroutine_ctx_t *g_coroutine_map[1024];


static coroutine_ctx_t*
coroutine_get_ctx(coroutine_t cid)
{
    return g_coroutine_map[cid];
}


void coroutine_init_main()
{
    coroutine_t cid;
    coroutine_ctx_t *main_ctx;
    if (g_coroutine_map[0] == NULL) {
        coroutine_init_cidmap();
        cid = coroutine_get_free_cid();

        main_ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
        getcontext(&main_ctx->ctx);
        g_coroutine_map[cid] = main_ctx;
    }
}


int coroutine_create(coroutine_t *cidp, const void *attr,
    void*(*start_rtn)(void*), void *arg)
{
    coroutine_t cid;
    (void)attr;

    coroutine_init_main();

    cid = coroutine_get_free_cid();
    if (cid == -1) {
        return -1;
    }

    g_coroutine_map[cid] = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
    g_coroutine_map[cid]->stk = (u_char*)mmap(NULL, 8192,
          PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    getcontext(&g_coroutine_map[cid]->ctx);
    g_coroutine_map[cid]->ctx.uc_stack.ss_sp = g_coroutine_map[cid]->stk;
    g_coroutine_map[cid]->ctx.uc_stack.ss_size = 8192;
    g_coroutine_map[cid]->ctx.uc_link = &g_coroutine_map[0]->ctx;
    makecontext(&g_coroutine_map[cid]->ctx, (void(*)())start_rtn, 1, arg);

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

