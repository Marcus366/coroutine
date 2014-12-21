#ifndef __SCHED_H__
#define __SCHED_H__


#include "coroutine.h"
#include "context.h"


typedef struct fdstat {
  unsigned open : 1;

  int fl;

  list_head wq;
} fdstat_t;


extern fdstat_t        *g_fds;
extern coroutine_ctx_t *g_exit_coroutine_ctx;


int coroutine_sched_init();


int coroutine_sched_regfd(int fd, int fl);
int coroutine_sched_unregfd(int fd);


void coroutine_sched_block(coroutine_ctx_t *ctx, int fd, int type);

void coroutine_sched();


void coroutine_sched_swap_context(coroutine_ctx_t *cur, coroutine_ctx_t *next);

void* coroutine_exit(void *arg);


#endif

