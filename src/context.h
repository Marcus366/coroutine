#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdint.h>
#include <ucontext.h>
#include "list.h"


typedef enum crt_state_s {
    READY = 0,
    RUNNING,
    BLOCKING,
    ZOMBIE
} crt_state_t;


typedef struct crt_ctx_s crt_ctx_t;
struct crt_ctx_s {
    unsigned long long  cid;
    crt_state_t   flag;
    crt_ctx_t    *parent;

    /* stack message */
    void          *stack_base;
    void          *stack_pointer;
    size_t         stack_size;

    /* crt list */
    list_head       list;
    /*   wait queue   */
    list_head       queue;

    void (*start_rtn)(void*);
    void *arg;
};


extern list_head  g_crt_list;
extern list_head  g_crt_ready_list;
extern list_head  g_crt_zombie_list;
extern crt_ctx_t *g_crt_running_ctx;


crt_ctx_t* crt_ctx_new(void(*func)(), void *arg);

crt_ctx_t* crt_ctx_new_main();
crt_ctx_t* crt_ctx_new_exit();


void crt_ctx_free(crt_ctx_t *ctx);


#endif

