#include <stdlib.h>
#include <assert.h>
#include "waitqueue.h"

#include <stdio.h>

wq_item_t*
wqitem_new(coroutine_ctx_t *ctx)
{
  wq_item_t *item = (wq_item_t*)malloc(sizeof(wq_item_t));

  if (item == NULL) {
    return NULL;
  }

  item->ctx = ctx;

  return item;
}


void
wqitem_free(wq_item_t *item)
{
  assert(!list_is_suspend(&item->queue));

  list_del(&item->queue);
  free(item);
}

