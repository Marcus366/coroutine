#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/mman.h>

#include "log.h"
#include "list.h"
#include "sched.h"
#include "context.h"
#include "coroutine.h"


void
crt_init()
{
    crt_ctx_t *ctx;

    crt_sched_init();

    /* init main crt */
    ctx = crt_ctx_new_main();
    list_add(&ctx->list, &g_crt_list);

    g_crt_running_ctx = ctx;
}


crt_ctx_t*
crt_create(const void *attr,
        void(*start_rtn)(void*), void *arg)
{
    crt_ctx_t *ctx;

    /* TODO:
     * corutine attribute.
     */
    (void)attr;

    ctx = crt_ctx_new(start_rtn, arg);

    /* add to crt list */
    list_add(&ctx->list, &g_crt_list);
    /* add to ready queue */
    list_add_tail(&ctx->queue, &g_crt_ready_list);


#ifdef __DEBUG__
    printf("make crt cid: %" PRIu64 "\n", ctx->cid);
#endif

    return ctx;
}


void
crt_resume(crt_ctx_t *ctx)
{
    crt_ctx_t *cur = g_crt_running_ctx;

    cur->flag = READY;
    ctx->flag = RUNNING;
    g_crt_running_ctx = ctx;

    list_add_tail(&cur->queue, &g_crt_ready_list);

    /* if the next crt is in ready list, remove it */
    if (!list_is_suspend(&ctx->queue)) {
        list_del(&ctx->queue);
    }

#ifdef __DEBUG__
    printf("resume crt cid: %" PRIu64 "\n", ctx->cid);
#endif

    crt_sched_swap_context(cur, ctx);
}


void
crt_yield()
{
    crt_ctx_t *ctx;

    ctx = g_crt_running_ctx;

    ctx->flag = READY;
    list_add_tail(&ctx->queue, &g_crt_ready_list);

    crt_sched();
}


crt_ctx_t*
crt_running()
{
    return g_crt_running_ctx;
}

