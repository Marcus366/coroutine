#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdint.h>
#include <ucontext.h>
#include "list.h"


typedef enum coroutine_state_s {
  READY = 0,
  RUNNING,
  BLOCKING
} coroutine_state_t;


typedef struct coroutine_ctx_s coroutine_ctx_t;
struct coroutine_ctx_s {
  unsigned long long  cid;
  coroutine_state_t   flag;
  coroutine_ctx_t    *parent;
  ucontext_t          ctx;

  /* coroutine list */
  list_head       list;
  /*   wait queue   */
  list_head       queue;
};


extern list_head  g_coroutine_list;
extern list_head  g_coroutine_ready_list;
extern coroutine_ctx_t *g_coroutine_running_ctx;


coroutine_ctx_t* coroutine_ctx_new(void(*func)(), void *arg);

coroutine_ctx_t* coroutine_ctx_new_main();
coroutine_ctx_t* coroutine_ctx_new_exit();


void coroutine_ctx_free(coroutine_ctx_t *ctx);


#endif

