#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/coroutine.h"

coroutine_t c[100];


void* foo(void *arg) {
    coroutine_t cid = (long)arg;
    coroutine_resume((cid + 1) % 100);

    return NULL;
}


int main()
{
    long i;
    printf("switch_test begin\n");
    for (i = 0; i < 100; ++i) {
        coroutine_create(&c[i], NULL, foo, (void*)i);
    }
    coroutine_resume(c[0]);
    printf("switch_test end\n");

    return 0;
}
