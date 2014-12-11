#ifndef __CIDMAP_H__
#define __CIDMAP_H__


#include "bitset.h"
#include "coroutine.h"
#include <stdint.h>


typedef struct cidmap {
  struct cidmap *next;
  uint32_t       used;

  /* Bitmap of used/unused cid.
   * Make the bitmap's size be 4K for memory alignment.
   */
  ull           *map;
} cidmap_t;

#define bits_per_map (1024 * 4 * 8)

void coroutine_cidmap_init();


coroutine_t coroutine_get_free_cid();
void        coroutine_erase_cid(coroutine_t cid);


#endif

