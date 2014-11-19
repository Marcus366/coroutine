#ifndef __CIDMAP_H__
#define __CIDMAP_H__


#include "bitset.h"
#include "coroutine.h"
#include <stdint.h>


typedef struct cidmap {
    /* make the whole cid_map struct size be 4K */
    struct __packed__ {
        struct cidmap *next;
        uint32_t used;
    } __packed__;
#define next __packed__.next
#define used __packed__.used

    /* bitmap of used/unused cid */
    DEFINE_BITSET(set, 1024 * 4 * 8 - sizeof(struct __packed__) * 8);
} cidmap_t;


static cidmap_t *g_cidmap_head;


void coroutine_init_cidmap();


coroutine_t coroutine_get_free_cid();
coroutine_t coroutine_set_cid();
coroutine_t coroutine_get_set_free_cid();

#undef next
#undef used

#endif
