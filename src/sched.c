#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <sys/mman.h>

#include "list.h"
#include "utils.h"
#include "sched.h"
#include "config.h"
#include "context.h"


coroutine_t g_exit_coroutine;
fdstat_t *g_fds;

static int g_pollfd;
static void* coroutine_exit_sched(void *arg);


int
coroutine_sched_init() {
  struct rlimit rlim;

  if (coroutine_create(&g_exit_coroutine, NULL,
      coroutine_exit_sched, NULL) == -1)
  {
    return -1;
  }

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


void
coroutine_sched_block(coroutine_ctx_t *ctx, int fd, int type)
{
  struct epoll_event ev;

  ctx->flag = BLOCKING;

  ev.events = type;
  ev.data.ptr = ctx;

  epoll_ctl(g_pollfd, EPOLL_CTL_ADD, fd, &ev);

  coroutine_sched();
}


void
coroutine_sched()
{
  int i, nfds;
  coroutine_t cid, prev;
  coroutine_ctx_t *prev_ctx, *ctx;
  struct epoll_event event[1024];

  cid = -1;
  nfds = epoll_wait(g_pollfd, event, 1024, 0);
  for (i = 0; i < nfds; ++i) {
    ctx = (coroutine_ctx_t*)event[i].data.ptr;
    assert(ctx->flag == BLOCKING);
    ctx->flag = READY;

    assert(list_is_suspend(&ctx->queue));
    list_add_tail(&ctx->queue, &g_coroutine_ready_list);
  }

  list_for_each_entry(ctx, &g_coroutine_ready_list, queue) {
    if (ctx->flag == READY && ctx->cid != g_exit_coroutine && ctx->cid != coroutine_self()) {
      cid = ctx->cid;
      break;
    }
  }

  if (cid != -1) {
    goto do_sched;
  }

  nfds = epoll_wait(g_pollfd, event, 1024, -1);
  for (i = 0; i < nfds; ++i) {
    ctx = (coroutine_ctx_t*)event[i].data.ptr;
    assert(ctx->flag == BLOCKING);
    ctx->flag = READY;

    assert(list_is_suspend(&ctx->queue));
    list_add_tail(&ctx->queue, &g_coroutine_ready_list);

    cid = ctx->cid;
  }

do_sched:
#ifdef __DEBUG_SHOW_ALL_LIST__
  printf("list:\n");
  list_for_each_entry(ctx, &g_coroutine_list, list) {
    printf("%ld ", ctx->cid);
    switch (ctx->flag) {
    case RUNNING:
      printf("running\n");
      if (!list_is_suspend(&ctx->queue)) {
        printf("state not consistent\n");
        exit(-1);
      }
      break;
    case BLOCKING:
      printf("blocking\n");
      if (!list_is_suspend(&ctx->queue)) {
        printf("state not consistent\n");
        exit(-1);
      }
      break;
    case READY:
      printf("ready\n");
      if (ctx->cid != g_exit_coroutine && list_is_suspend(&ctx->queue)) {
        printf("state not consistent\n");
        exit(-1);
      }
      break;
    default:
      printf("\n");
    }
  }
#endif

#ifdef __DEBUG_SHOW_READY_LIST__
  printf("ready list:\n");
  list_for_each_entry(ctx, &g_coroutine_ready_list, queue) {
    printf("%ld ", ctx->cid);
    switch (ctx->flag) {
    case RUNNING:
      printf("state not consistent\n");
      exit(-1);
    case BLOCKING:
      printf("state not consistent\n");
      exit(-1);
    case READY:
      if (ctx->cid == g_exit_coroutine) {
        printf("state not consistent\n");
        exit(-1);
      }
      break;
    }
  }
  printf("\n");
#endif

  if (cid == -1) {
    printf("no coroutine can be sched!\n");
    exit(0);
  }

#ifdef __DEBUG__
  printf("sched coroutine cid: %ld\n", cid);
#endif

  prev = coroutine_self();
  prev_ctx = coroutine_get_ctx(prev);
  ctx = coroutine_get_ctx(cid);

  list_del(&ctx->queue);
  ctx->flag = RUNNING;
  g_coroutine_running = cid;

  swapcontext(&prev_ctx->ctx, &ctx->ctx);

  return;
}


void*
coroutine_exit_sched(void *arg)
{
  coroutine_t cid;
  coroutine_ctx_t *ctx;
  for (;;) {
    /*
     * Next line will get the last running coroutine id,
     * NOT the coroutine id of exit_sched.
     */
    cid = coroutine_self();
    ctx = coroutine_get_ctx(cid);

    g_coroutine_running = g_exit_coroutine;

    /* clean up correspond bit of cid bitmap. */
    coroutine_earse_cid(cid);

    assert(list_is_suspend(&ctx->queue));
    list_del(&ctx->list);
    hlist_del(&ctx->hash);
    munmap(ctx->ctx.uc_stack.ss_sp, ctx->ctx.uc_stack.ss_size);
    free(ctx);

#ifdef __DEBUG__
    printf("coroutine exit: %ld\n", cid);
#endif

    coroutine_sched();
  }

  return NULL;
}

