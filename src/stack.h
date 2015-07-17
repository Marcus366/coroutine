#ifndef __CRT_STACK_H__
#define __CRT_STACK_H__


#include <stddef.h>
#include "list.h"


typedef struct crt_stack_s {
    void       *base;
    void       *pointer;
    size_t      size;
    unsigned    main:1;
} crt_stack_t;


typedef struct crt_stack_node_s {
    crt_stack_t    stk;
    list_head      node;
} crt_stack_node_t;


crt_stack_t crt_get_stack();
void crt_put_stack(crt_stack_t stk);


#endif

