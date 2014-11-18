#include "coroutine.h"
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>


typedef struct {
    ucontext_t ctx;
    u_char    *stk;
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
    coroutine_ctx_t *main_ctx;
    if (g_coroutine_map[0] == NULL) {
        main_ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
        
        getcontext(&main_ctx->ctx);

        g_coroutine_map[0] = main_ctx;
    }
}


int coroutine_create(coroutine_t *cidp, const void *attr,
    void*(*start_rtn)(void*), void *arg)
{
    int i;
    (void)attr;

    coroutine_init_main();

    for (i = 1; i < 1024; ++i) {
        if (g_coroutine_map[i] == NULL) {
            g_coroutine_map[i] = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
            g_coroutine_map[i]->stk = (u_char*)malloc(8196);

            getcontext(&g_coroutine_map[i]->ctx);
            g_coroutine_map[i]->ctx.uc_stack.ss_sp = g_coroutine_map[i]->stk;
            g_coroutine_map[i]->ctx.uc_stack.ss_size = 8196;
            g_coroutine_map[i]->ctx.uc_link = &g_coroutine_map[0]->ctx;
            makecontext(&g_coroutine_map[i]->ctx, (void(*)())start_rtn, 1, arg);

            printf("make coroutine cid: %d\n", i);

            *cidp = i;
            return 0;
        }
    }

    return -1;
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


coroutine_t
coroutine_self()
{
    return g_coroutine_running;
}

