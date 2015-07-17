#include <sys/mman.h>
#include <stdint.h>

#include "stack.h"
#include "pool.h"
#include "context.h"


#define STACK_SIZE (8192 * 1024)
#define STK_FREELIST_SIZE (128)


static list_head g_stk_freelist = LIST_HEAD_INIT(g_stk_freelist);
static int       g_stk_freelist_count = 0;


static crt_stack_t crt__get_stack();


crt_stack_t
crt_get_stack()
{
    char        *sp, *ebp;
    crt_stack_t  stk;

    stk = crt__get_stack();

    /* Add the corutine entrance to top of stack */
    sp = (char*)stk.base + stk.size - 1;
    sp = (char*)((unsigned long)sp & (-16L));

    ebp = sp;
    sp -= 8;
    *(uintptr_t*)sp = (uintptr_t)crt_enter;

    sp -= 8;
    *(uintptr_t*)sp  = (uintptr_t)ebp;
    sp -= 6 * 8;

    stk.pointer = sp;

    return stk;
}


void
crt_put_stack(crt_stack_t stk)
{
    /*
     * This function only called when a coroutine exiting.
     * At this time if freelist is full, we cannot free
     * the stack because the exiting coroutine is running on
     * this stack. Instead we free another stack.
     */
    crt_stack_node_t *del, *node;

    if (stk.main) {
        return ;
    }

    node = (crt_stack_node_t*)malloc(sizeof(crt_stack_node_t));
    node->stk = stk;

    if (g_stk_freelist_count >= STK_FREELIST_SIZE) {
        del = (crt_stack_node_t*)
            list_first_entry(&g_stk_freelist, crt_stack_node_t, node);
        list_del(&del->node);
        --g_stk_freelist_count;

        /* free the stack space */
        munmap(del->stk.base, del->stk.size);
        /* free the stack struct */
        /* TODO: use pool to manage stack struct */
        free(del);
    }

    list_add(&node->node, &g_stk_freelist);
    ++g_stk_freelist_count;
}


crt_stack_t
crt__get_stack()
{
    void             *base;
    size_t            size;
    crt_stack_t       stk;
    crt_stack_node_t *entry;

    if (g_stk_freelist_count != 0) {
        entry = (crt_stack_node_t*)
            list_first_entry(&g_stk_freelist, crt_stack_node_t, node);

        stk.base = entry->stk.base;
        stk.size = entry->stk.size;
        stk.main = 1;

        list_del(&entry->node);
        --g_stk_freelist_count;

        return stk;
    }

    /* There is no stack in freelist, so initilize one below. */
    size = STACK_SIZE;
    base = (u_char*)mmap(NULL, size,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    stk.base = base;
    stk.size = size;
    stk.main = 1;

    return stk;
}

