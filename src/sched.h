#ifndef __SCHED_H__
#define __SCHED_H__

#include "coroutine.h"

#define SCHED_BLOCK -1
#define SCHED_NOWAIT 0

typedef struct fdstat {
    unsigned open : 1;
    int fl;
} fdstat_t;


extern fdstat_t *fds;


int coroutine_sched_init();


int coroutine_sched_regfd(int fd);
int coroutine_sched_unregfd(int fd);


coroutine_t coroutine_sched(int type);


#endif

