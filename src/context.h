#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdint.h>
#include <ucontext.h>
#include "list.h"
#include "stack.h"


typedef enum crt_state_s {
    READY = 0,
    RUNNING,
    BLOCKING,
    ZOMBIE
} crt_state_t;


typedef struct crt_ctx_s crt_ctx_t;
struct crt_ctx_s {
    uint64_t      cid;
    crt_state_t   flag;
    crt_ctx_t    *parent;

    /* stack message */
    crt_stack_t    stack;

    /* coroutine list, all coroutines link in one list */
    list_head       list;
    /* wait queue or ready queue */
    list_head       queue;

    void (*func)(void*);
    void *arg;
};


extern list_head  g_crt_list;
extern list_head  g_crt_ready_list;
extern list_head  g_crt_zombie_list;
extern crt_ctx_t *g_crt_running_ctx;


crt_ctx_t* crt_ctx_new(void(*func)(void*), void *arg);

crt_ctx_t* crt_ctx_new_main();
crt_ctx_t* crt_ctx_new_exit();

void crt_enter();

void crt_ctx_free(crt_ctx_t *ctx);


#endif

