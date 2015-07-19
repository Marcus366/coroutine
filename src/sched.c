#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/time.h>
#include <inttypes.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <sys/mman.h>

#include "log.h"
#include "list.h"
#include "utils.h"
#include "sched.h"
#include "context.h"


fdstat_t        *g_fds;
int              g_pollfd;
crt_ctx_t *g_exit_crt_ctx;


extern void __attribute__((noinline))
    crt__swap_context(void **from, void **to);
    //asm("crt__swap_context");


int
crt_sched_init() {
    int i;
    struct rlimit rlim;

    /*
    if ((g_exit_crt_ctx = crt_ctx_new_exit()) == NULL) {
        return -1;
    }
    */

    if (getrlimit(RLIMIT_NOFILE, &rlim) != 0) {
        return -1;
    }

    g_fds = (fdstat_t*)calloc(sizeof(fdstat_t), rlim.rlim_max);
    if (g_fds == NULL) {
        return -1;
    }

    for (i = 0; i < (int)rlim.rlim_max; ++i) {
        INIT_LIST_HEAD(&g_fds[i].wq);
    }

    if ((g_pollfd = epoll_create1(EPOLL_CLOEXEC)) == -1) {
        return -1;
    }

    return 0;
}


int
crt_register_fd(int fd)
{
    return 0;
}


int
crt_unregister_fd(int fd)
{
    return 0;
}


void
crt_block(int fd, int type)
{
    struct epoll_event ev;
    crt_ctx_t *ctx;

    ctx = g_crt_running_ctx;
    ctx->flag = BLOCKING;

    assert(list_is_suspend(&ctx->queue));
    list_add_tail(&ctx->queue, &g_fds[fd].wq);

    ev.events = type;
    ev.data.fd = fd;

    epoll_ctl(g_pollfd, EPOLL_CTL_ADD, fd, &ev);

    crt_sched();
}


void
crt_sched()
{
    crt_ctx_t *ctx, *cur;

    ctx = crt_sched_find_ready();

#ifdef __DEBUG__
    CRT_LOG_LIST;
    CRT_LOG_READY_LIST;
    printf("sched crt cid: %" PRIu64 "\n", ctx->cid);
#endif

    cur = g_crt_running_ctx;
    if (cur->cid != ctx->cid) {
        g_crt_running_ctx = ctx;

        list_del(&ctx->queue);
        ctx->flag = RUNNING;

        crt_sched_swap_context(cur, ctx);
    } else {
        list_del(&cur->queue);
    }

    return;
}


crt_ctx_t*
crt_sched_find_ready()
{
    int i, fd, nfds;
    crt_ctx_t *ctx, *cur;
    struct epoll_event event[10240];

    do {
        list_for_each_entry(ctx, &g_crt_ready_list, queue) {
            if (ctx->flag == READY) {
                return ctx;
            }
        }

        nfds = epoll_wait(g_pollfd, event, 10240, 0);
        for (i = 0; i < nfds; ++i) {
            fd = (int)event[i].data.fd;
            list_for_each_entry_safe(ctx, cur, &g_fds[fd].wq, queue) {
                if (ctx->flag == BLOCKING) {
                    ctx->flag = READY;

                    list_del(&ctx->queue);
                    list_add_tail(&ctx->queue, &g_crt_ready_list);
                }
            }

            if (epoll_ctl(g_pollfd, EPOLL_CTL_DEL, fd, event) == -1) {
                perror("epoll del error");
                exit(-1);
            }

            assert(list_empty(&g_fds[fd].wq));
        }

        if (!list_empty(&g_crt_ready_list)) {
            continue;
        }

        nfds = epoll_wait(g_pollfd, event, 10240, -1);
        for (i = 0; i < nfds; ++i) {
            fd = (int)event[i].data.fd;
            list_for_each_entry_safe(ctx, cur, &g_fds[fd].wq, queue) {
                if (ctx->flag == BLOCKING) {
                    ctx->flag = READY;

                    list_del(&ctx->queue);
                    list_add_tail(&ctx->queue, &g_crt_ready_list);
                }
            }

            if (epoll_ctl(g_pollfd, EPOLL_CTL_DEL, fd, event) == -1) {
                perror("epoll del error");
                exit(-1);
            }

            assert(list_empty(&g_fds[fd].wq));
        }
    } while(1);

    return ctx;
}


void
crt_sched_swap_context(crt_ctx_t *cur, crt_ctx_t *next)
{
    crt__swap_context(&cur->stack.pointer, &next->stack.pointer);
}


void*
crt_exit(void *arg)
{
    crt_ctx_t *pctx, *ctx;
    for (;;) {
        /*
         * Next line will get the last running crt id,
         * NOT the crt id of exit_sched.
         */
        ctx = g_crt_running_ctx;
        pctx = ctx->parent;

        g_crt_running_ctx = g_exit_crt_ctx;

        assert(list_is_suspend(&ctx->queue));
        list_del(&ctx->list);
        crt_ctx_free(ctx);

#ifdef __DEBUG__
        printf("crt exit: %" PRIu64 "\n", ctx->cid);
#endif

        if (pctx != NULL /* && pctx->flag == READY */) {
            pctx->flag = RUNNING;
            g_crt_running_ctx = pctx;

            if (!list_is_suspend(&pctx->queue)) {
                list_del(&pctx->queue);
            }

            crt_sched_swap_context(g_exit_crt_ctx, pctx);
        }

        crt_sched();
    }

    return NULL;
}

