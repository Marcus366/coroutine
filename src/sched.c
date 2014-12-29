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


fdstat_t        *g_fds;
int              g_pollfd;
coroutine_ctx_t *g_exit_coroutine_ctx;


extern void coroutine__sched_swap_context(ucontext_t *from, ucontext_t *to);
  //asm("_coroutine__sched_swap_context");


int
coroutine_sched_init() {
  int i;
  struct rlimit rlim;

  if ((g_exit_coroutine_ctx = coroutine_ctx_new_exit()) == NULL) {
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
    INIT_LIST_HEAD(&g_fds[i].wq);
  }

  if ((g_pollfd = epoll_create(10240)) == -1) {
    return -1;
  }

  return 0;
}


int
coroutine_register_fd(int fd)
{
  return 0;
}


int
coroutine_unregister_fd(int fd)
{
  return 0;
}


void
coroutine_block(int fd, int type)
{
  struct epoll_event ev;
  coroutine_ctx_t *ctx;

  ctx = g_coroutine_running_ctx;
  ctx->flag = BLOCKING;

  assert(list_is_suspend(&ctx->queue));
  list_add_tail(&ctx->queue, &g_fds[fd].wq);

  ev.events = type;
  ev.data.fd = fd;

  epoll_ctl(g_pollfd, EPOLL_CTL_ADD, fd, &ev);

  coroutine_sched();
}


void
coroutine_sched()
{
  int i, fd, nfds;
  coroutine_ctx_t *ctx, *cur;
  struct epoll_event event[10240];

find_ready:
  list_for_each_entry(ctx, &g_coroutine_ready_list, queue) {
    if (ctx->flag == READY) {
      goto do_sched;
    }
  }

  nfds = epoll_wait(g_pollfd, event, 10240, -1);
  for (i = 0; i < nfds; ++i) {
    fd = (int)event[i].data.fd;
    list_for_each_entry_safe(ctx, cur, &g_fds[fd].wq, queue) {
      if (ctx->flag == BLOCKING) {
        ctx->flag = READY;

        list_del(&ctx->queue);
        list_add_tail(&ctx->queue, &g_coroutine_ready_list);
      }

    }

    if (epoll_ctl(g_pollfd, EPOLL_CTL_DEL, fd, event) == -1) {
      perror("epoll del error");
      exit(-1);
    }

    assert(list_empty(&g_fds[fd].wq));
  }
  goto find_ready;

do_sched:
#ifdef __DEBUG_SHOW_ALL_LIST__
  printf("list:\n");
  list_for_each_entry(cur, &g_coroutine_list, list) {
    printf("%ld ", cur->cid);
    switch (cur->flag) {
    case RUNNING:
      printf("running\n");
      if (!list_is_suspend(&cur->queue)) {
        printf("state not consistent\n");
        exit(-1);
      }
      break;
    case BLOCKING:
      printf("blocking\n");
      if (!list_is_suspend(&cur->queue)) {
        printf("state not consistent\n");
        exit(-1);
      }
      break;
    case READY:
      printf("ready\n");
      if (cur->cid != g_exit_coroutine_ctx->cid && list_is_suspend(&ctx->queue)) {
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
  list_for_each_entry(cur, &g_coroutine_ready_list, queue) {
    printf("%ld ", cur->cid);
    switch (cur->flag) {
    case RUNNING:
      printf("state not consistent\n");
      exit(-1);
    case BLOCKING:
      printf("state not consistent\n");
      exit(-1);
    case READY:
      if (cur->cid == g_exit_coroutine_ctx->cid) {
        printf("state not consistent\n");
        exit(-1);
      }
      break;
    }
  }
  printf("\n");
#endif

#ifdef __DEBUG__
  printf("sched coroutine cid: %llu\n", ctx->cid);
#endif

  cur = g_coroutine_running_ctx;
  if (cur->cid != ctx->cid) {
    g_coroutine_running_ctx = ctx;

    list_del(&ctx->queue);
    ctx->flag = RUNNING;

    coroutine_sched_swap_context(cur, ctx);
  }

  return;
}


asm (
".text\n"
"coroutine__sched_swap_context:\n"
"pushq %r12\n"
"pushq %r13\n"
"pushq %r14\n"
"pushq %r15\n"
"pushq %rbx\n"
"pushq %rbp\n"

"movq %rsp, 160(%rdi)\n"
"movq 160(%rsi), %rsp\n"

"popq %rbp\n"
"popq %rbx\n"
"popq %r15\n"
"popq %r14\n"
"popq %r13\n"
"popq %r12\n"

"ret\n"
);


void
coroutine_sched_swap_context(coroutine_ctx_t *cur, coroutine_ctx_t *next)
{
  coroutine__sched_swap_context(&cur->ctx, &next->ctx);
}


void*
coroutine_exit(void *arg)
{
  coroutine_ctx_t *pctx, *ctx;
  for (;;) {
    /*
     * Next line will get the last running coroutine id,
     * NOT the coroutine id of exit_sched.
     */
    ctx = g_coroutine_running_ctx;
    pctx = ctx->parent;

    g_coroutine_running_ctx = g_exit_coroutine_ctx;

    assert(list_is_suspend(&ctx->queue));
    list_del(&ctx->list);
    munmap(ctx->ctx.uc_stack.ss_sp, ctx->ctx.uc_stack.ss_size);
    free(ctx);

#ifdef __DEBUG__
    printf("coroutine exit: %llu\n", ctx->cid);
#endif

    if (pctx != NULL /* && pctx->flag == READY */) {
        pctx->flag = RUNNING;
        g_coroutine_running_ctx = pctx;

        if (!list_is_suspend(&pctx->queue)) {
          list_del(&pctx->queue);
        }

        coroutine_sched_swap_context(g_exit_coroutine_ctx, pctx);
    }

    coroutine_sched();
  }

  return NULL;
}

