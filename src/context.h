#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdint.h>
#include <ucontext.h>
#include "coroutine.h"
#include "list.h"


#define READY      0
#define RUNNING    1
#define BLOCKING   2


typedef struct hlist_head hlist_head;
typedef struct hlist_node hlist_node;
typedef struct {
    coroutine_t  cid;
    u_char      *stk;
    ucontext_t   ctx;
    hlist_node   hash;

    uint32_t     flag;
} coroutine_ctx_t;


extern hlist_head g_coroutine_map[1024];


coroutine_ctx_t* coroutine_get_ctx(coroutine_t cid);


void coroutine_set_ctx(coroutine_t cid, coroutine_ctx_t *ctx);


#endif

