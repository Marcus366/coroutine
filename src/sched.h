#ifndef __SCHED_H__
#define __SCHED_H__


#include "coroutine.h"
#include "context.h"


typedef struct fdstat {
  list_head wq;
} fdstat_t;


extern fdstat_t        *g_fds;
extern coroutine_ctx_t *g_exit_coroutine_ctx;


int coroutine_sched_init();


int coroutine_register_fd(int fd);
int coroutine_unregister_fd(int fd);


void coroutine_block(int fd, int type);

void coroutine_sched();

coroutine_ctx_t* coroutine_sched_find_ready();


void coroutine_sched_swap_context(coroutine_ctx_t *cur, coroutine_ctx_t *next);

void* coroutine_exit(void *arg);


#endif

