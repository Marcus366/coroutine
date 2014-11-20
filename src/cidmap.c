#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cidmap.h"


static cidmap_t *g_cidmap_head;

void
coroutine_init_cidmap()
{
    assert(g_cidmap_head == NULL);

    /* TODO:
     * A test for whether malloc of mmap is faster.
     */
    g_cidmap_head = (cidmap_t*)calloc(1, sizeof(cidmap_t));
}


coroutine_t
coroutine_get_free_cid()
{
    unsigned long i, j, map_offset;
    coroutine_t ret;
    cidmap_t *cidmap;

    ret = 0;
    cidmap = g_cidmap_head;
    while (cidmap != NULL) {
        if (cidmap->used == bits_per_map) {
            cidmap = cidmap->next;
            ret += bits_per_map;
            continue;
        }

        for (i = 0; i < bits_per_map / ull_bits; ++i) {
            if (cidmap->map[i] == ~(0ull)) {
                continue;
            }

            for (j = 0; j < ull_bits; ++j) {
                if (!(cidmap->map[i] & ((1ull) << j))) {
                    map_offset = i * ull_bits + j;
                    BITSET_SETBIT(cidmap->map, map_offset, 1);
                    cidmap->used++;
                    return ret + map_offset;
                }
            }

        }
    }

    cidmap->next = (cidmap_t*)calloc(1, sizeof(cidmap_t));
    cidmap = cidmap->next;
    BITSET_SETBIT(cidmap->map, 0, 1);

    return ret;
}
