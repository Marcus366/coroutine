#ifndef __SCHED_H__
#define __SCHED_H__


#include "coroutine.h"
#include "context.h"


typedef struct fdstat {
    list_head wq;
} fdstat_t;


extern fdstat_t        *g_fds;
extern crt_ctx_t *g_exit_crt_ctx;


int crt_sched_init();


int crt_register_fd(int fd);
int crt_unregister_fd(int fd);


void crt_block(int fd, int type);

void crt_sched();

crt_ctx_t* crt_sched_find_ready();


void crt_sched_swap_context(crt_ctx_t *cur, crt_ctx_t *next);

void* crt_exit(void *arg);


#endif

