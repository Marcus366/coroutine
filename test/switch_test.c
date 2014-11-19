#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/coroutine.h"


void* foo(void *arg) {
    coroutine_t cid = (long)arg;
    coroutine_resume((cid % 2) + 1);

    read(0, 0, 0);

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
