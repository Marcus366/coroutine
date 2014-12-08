#ifndef __SCHED_H__
#define __SCHED_H__


#include "coroutine.h"
#include "context.h"


typedef struct fdstat {
  unsigned open : 1;
  int fl;
} fdstat_t;


extern coroutine_t  g_exit_coroutine;
extern fdstat_t    *g_fds;


int coroutine_sched_init();


int coroutine_sched_regfd(int fd);
int coroutine_sched_unregfd(int fd);


void coroutine_sched_block(coroutine_ctx_t *ctx, int fd, int type);

void coroutine_sched();


#endif

