#ifndef __WAITQUEUE_H__
#define __WAITQUEUE_H__


#include "list.h"
#include "context.h"

typedef struct {
  list_head queue;

  coroutine_ctx_t *ctx;
} wq_item_t;


wq_item_t* wqitem_new(coroutine_ctx_t *ctx);
void       wqitem_free(wq_item_t *item);

#endif

