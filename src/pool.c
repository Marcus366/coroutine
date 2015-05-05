#include "pool.h"
#include <stdlib.h>

pool_t*
coroutine_pool_create(unsigned size, unsigned count) {
  if (size < sizeof(void*)) {
    size = sizeof(void*);
  }

  /* TODO:
   * Maybe we should limit the size.
   */
  pool_t *pool = (pool_t*)malloc(sizeof(pool_t) + size * count);
  if (pool != NULL) {
    pool->start    = (char*)pool + sizeof(pool_t);
    pool->freelist = (obj_t*)pool->start;

    pool->size  = size;
    pool->count = count;
    pool->inuse = 0;

    /* initilize the freelist */
    obj_t *p = pool->freelist;
    while (--count) {
      p->next = (obj_t*)((char*)p + size);
      p       = p->next;
    }
  }

  return pool;
}


int
coroutine_pool_destroy(pool_t *pool) {
  if (pool->inuse) {
    return -1;
  }

  free(pool);
  return 0;
}


void*
coroutine_pool_borrow(pool_t *pool) {
  if (pool->inuse >= pool->count) {
    return NULL;
  }

  obj_t *obj = pool->freelist;
  pool->freelist = obj->next;

  pool->inuse++;

  return obj;
}



void
coroutine_pool_release(pool_t *pool, void *obj) {
  if (obj < pool->start || obj > (char*)pool->start + pool->count * pool->size) {
    return;
  }

  pool->inuse--;
  
  obj_t *o = (obj_t*)obj;
  o->next = pool->freelist;
  pool->freelist = o;
}


