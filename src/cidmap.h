#ifndef __CIDMAP_H__
#define __CIDMAP_H__


#include "bitset.h"
#include "coroutine.h"
#include <stdint.h>


typedef struct cidmap {
    struct cidmap *next;
    uint32_t used;

    /* Bitmap of used/unused cid.
     * Make the whole cid_map struct size be 4K.
     */
#define bits_per_map \
    (1024 * 4 * 8 - (sizeof(struct cidmap*) + sizeof(uint32_t)) * 8)
    DEFINE_BITSET(map, bits_per_map);
} cidmap_t;


void coroutine_init_cidmap();


coroutine_t coroutine_get_free_cid();


#endif

