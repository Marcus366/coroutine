#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <sys/resource.h>

#include "utils.h"
#include "sched.h"
#include "context.h"


fdstat_t *g_fds;

static int g_pollfd;


int
coroutine_sched_init() {
    struct rlimit rlim;

    if (getrlimit(RLIMIT_NOFILE, &rlim) != 0) {
        return -1;
    }

    g_fds = (fdstat_t*)malloc(sizeof(fdstat_t*) * rlim.rlim_max);
    if (g_fds == NULL) {
        return -1;
    }

    if ((g_pollfd = epoll_create(1024)) == -1) {
        return -1;
    }

    return 0;
}


int
coroutine_sched_regfd(int fd)
{
    int fl;

    if ((fl = getfl(fd)) == -1) {
        return -1;
    }

    if (setfl(fd, fl | O_NONBLOCK) == -1) {
        return -1;
    }

    g_fds[fd].open = 1;
    g_fds[fd].fl = fl;
    return 0;
}


int
coroutine_sched_unregfd(int fd)
{
    g_fds[fd].open = 0;
    return 0;
}


coroutine_t
coroutine_sched(int type)
{
    int i, nfds;
    coroutine_t cid;
    coroutine_ctx_t *ctx;
    struct epoll_event event[1024];

    cid = -1;
    nfds = epoll_wait(g_pollfd, event, 1024, type);
    for (i = 0; i < nfds; ++i) {
        ctx = (coroutine_ctx_t*)event[i].data.ptr;
        ctx->flag = READY;
        cid = ctx->cid;
    }

    list_for_each_entry(ctx, &g_coroutine_list, list) {
        if (ctx->flag == READY && ctx->cid != coroutine_self()) {
            cid = ctx->cid;
            break;
        }
    }


    return cid;
}

