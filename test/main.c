#include <stdio.h>
#include <stdlib.h>
#include "../src/coroutine.h"


void* foo(void *arg) {
    long cid = (long)arg;
    printf("coroutine %ld running\n", cid);
    coroutine_resume((cid % 2) + 1);

    return NULL;
}


int main()
{
    coroutine_t c1, c2;
    coroutine_create(&c1, NULL, foo, (void*)1);
    coroutine_create(&c2, NULL, foo, (void*)2);
    coroutine_resume(c1);

    return 0;
}
