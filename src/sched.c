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
#include "cidmap.h"
#include "config.h"
#include "context.h"


fdstat_t *g_fds;
coroutine_t g_exit_coroutine;
coroutine_ctx_t *g_exit_coroutine_ctx;

static int   g_pollfd;
static int   coroutine_exit_create(coroutine_t *cid);
static void* coroutine_exit_sched(void *arg);


int
coroutine_sched_init() {
  int i;
  struct rlimit rlim;

  if (coroutine_exit_create(&g_exit_coroutine) == -1) {
    return -1;
  }

  if (getrlimit(RLIMIT_NOFILE, &rlim) != 0) {
    return -1;
  }

  g_fds = (fdstat_t*)malloc(sizeof(fdstat_t) * rlim.rlim_max);
  if (g_fds == NULL) {
    return -1;
  }

  for (i = 0; i < (int)rlim.rlim_max; ++i) {
    g_fds[i].open = 0;
    INIT_LIST_HEAD(&g_fds[i].wq);
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
  wq_item_t *item;
  struct epoll_event ev;

  ctx->flag = BLOCKING;

  item = wqitem_new(ctx);
  list_add_tail(&item->queue, &g_fds[fd].wq);

  ev.events = type;
  ev.data.fd = fd;

  epoll_ctl(g_pollfd, EPOLL_CTL_ADD, fd, &ev);

  coroutine_sched();
}


void
coroutine_sched()
{
  int i, fd, nfds;
  wq_item_t *item, *nitem;
  coroutine_t cid, prev;
  coroutine_ctx_t *ctx, *nextctx;
  struct epoll_event event[10240];

  cid = -1;
  nfds = epoll_wait(g_pollfd, event, 10240, 0);
  for (i = 0; i < nfds; ++i) {
    fd = (int)event[i].data.fd;
    list_for_each_entry_safe(item, nitem, &g_fds[fd].wq, queue) {
      ctx = item->ctx;

      assert(ctx->flag == BLOCKING);
      ctx->flag = READY;

      assert(list_is_suspend(&ctx->queue));
      list_add_tail(&ctx->queue, &g_coroutine_ready_list);

      wqitem_free(item);

      if (epoll_ctl(g_pollfd, EPOLL_CTL_DEL, fd, event) == -1) {
        printf("epoll del error");
        exit(-1);
      }
    }

    assert(list_empty(&g_fds[fd].wq));
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

  nfds = epoll_wait(g_pollfd, event, 10240, -1);
  for (i = 0; i < nfds; ++i) {
    fd = (int)event[i].data.fd;
    list_for_each_entry_safe(item, nitem, &g_fds[fd].wq, queue) {
      ctx = item->ctx;

      assert(ctx->flag == BLOCKING);
      ctx->flag = READY;
      
      assert(list_is_suspend(&ctx->queue));
      list_add_tail(&ctx->queue, &g_coroutine_ready_list);

      wqitem_free(item);

      cid = ctx->cid;

      if (epoll_ctl(g_pollfd, EPOLL_CTL_DEL, fd, event) == -1) {
        printf("epoll del error");
        exit(-1);
      }
    }

    assert(list_empty(&g_fds[fd].wq));
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
  ctx = coroutine_get_ctx(prev);
  nextctx = coroutine_get_ctx(cid);

  list_del(&nextctx->queue);
  nextctx->flag = RUNNING;
  g_coroutine_running = cid;
  g_coroutine_running_ctx = nextctx;

  coroutine_sched_swap_context(ctx, nextctx);

  return;
}


void
coroutine_sched_swap_context(coroutine_ctx_t *cur, coroutine_ctx_t *next)
{
  swapcontext(&cur->ctx, &next->ctx);
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
    g_coroutine_running_ctx = g_exit_coroutine_ctx;

    /* clean up correspond bit of cid bitmap. */
    coroutine_erase_cid(cid);

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


int
coroutine_exit_create(coroutine_t *cidp)
{
  coroutine_t cid;
  coroutine_ctx_t *ctx;

  cid = coroutine_get_free_cid();
  if (cid == -1) {
    return -1;
  }
  *cidp = cid;

  ctx = (coroutine_ctx_t*)malloc(sizeof(coroutine_ctx_t));
  coroutine_set_ctx(cid, ctx);

  ctx->cid  = cid;
  ctx->flag = READY;
  ctx->stk  = (u_char*)mmap(NULL, 8192000,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

  getcontext(&ctx->ctx);
  ctx->ctx.uc_stack.ss_sp = ctx->stk;
  ctx->ctx.uc_stack.ss_size = 8192000;
  ctx->ctx.uc_link = NULL;
  makecontext(&ctx->ctx, (void(*)())coroutine_exit_sched, 1, NULL);

  g_exit_coroutine_ctx = ctx;

#ifdef __DEBUG__
  printf("make exit coroutine cid: %ld\n", cid);
#endif

  return 0;
}

