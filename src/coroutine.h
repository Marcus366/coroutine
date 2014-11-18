#ifndef __COROUTINE_H__
#define __COROUTINE_H__

typedef int coroutine_t;

int coroutine_create(coroutine_t *cidp, const void *attr,
    void*(*start_rtn)(void*), void *arg);


void coroutine_resume(coroutine_t cid);
void coroutine_yield();


coroutine_t coroutine_self();


#endif
