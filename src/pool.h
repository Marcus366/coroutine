#ifndef __POOL_H__
#define __POOL_H__


typedef struct obj {
  struct obj *next;
} obj_t;


typedef struct pool {
  void  *start;
  obj_t *freelist;

  unsigned size;
  unsigned count;
  unsigned inuse;
} pool_t;


pool_t* coroutine_pool_create(unsigned size, unsigned count);
int     coroutine_pool_destroy(pool_t *pool);


void* coroutine_pool_borrow(pool_t *pool);
void  coroutine_pool_release(pool_t *pool, void *obj);


#endif

