#include "../src/crt.h"
#include "../src/pool.h"
#include <assert.h>
#include <stdio.h>


typedef struct {
  int a;
  int b;
  char c;
} foo;


foo *arr[100];


int main() {
  int i;
  pool_t *pool = crt_pool_create(sizeof(foo), 100);
  assert(pool->inuse == 0);

  for (i = 0; i < 100; ++i) {
    //printf("%d borrow\n", i);
    assert(pool->inuse == i);
    foo *bar = (foo*)crt_pool_borrow(pool);
    arr[i] = bar;
    assert(bar);
    assert(pool->inuse == i + 1);
  }

  //printf("borrow done\n");

  for (i = 100; i > 0; --i) {
    //printf("%d release\n", i);
    assert(pool->inuse == i);
    crt_pool_release(pool, arr[i - 1]);
    assert(pool->inuse == i - 1);
  }
  
  return 0;
}
