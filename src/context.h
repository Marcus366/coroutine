#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdint.h>
#include <ucontext.h>
#include "coroutine.h"
#include "list.h"


#define READY      0
#define RUNNING    1
#define BLOCKING   2


typedef struct coroutine_ctx coroutine_ctx_t;
typedef struct coroutine_ctx {
  coroutine_t      cid;
  coroutine_ctx_t *parent;
  u_char          *stk;
  ucontext_t       ctx;
  uint32_t         flag;

  hlist_node       hash;
  /* coroutine list */
  list_head        list;
  /*   wait queue   */
  list_head        queue;
};


extern hlist_head g_coroutine_map[1024];
extern list_head  g_coroutine_list;
extern list_head  g_coroutine_ready_list;
extern coroutine_ctx_t *g_coroutine_running_ctx;


coroutine_ctx_t* coroutine_get_ctx(coroutine_t cid);


void coroutine_set_ctx(coroutine_t cid, coroutine_ctx_t *ctx);


#endif

