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


pool_t* crt_pool_create(unsigned size, unsigned count);
int     crt_pool_destroy(pool_t *pool);


void* crt_pool_borrow(pool_t *pool);
void  crt_pool_release(pool_t *pool, void *obj);


#endif

