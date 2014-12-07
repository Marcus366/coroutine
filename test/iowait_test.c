#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../src/coroutine.h"


void* co_read(void *arg)
{
    (void) arg;
    char buf[1024];
    int n, fd = open("fifo", O_RDONLY, 0777);
    if (fd == -1) {
        printf("open failed failed\n");
        exit(-1);
    }

    for (;;) {
        while ((n = read(fd, buf, 1024)) > 0) {
            buf[n] = 0;
            printf("read: %s\n", buf);
        }
    }

    return NULL;
}


void co_idle()
{
    int i;

    for (;;) {
        for (i = 0; i < 10000; ++i) ;

        printf("idle sched\n");
        coroutine_yield();
    }
}


int main() {
    coroutine_t cid;

    if (coroutine_create(&cid, NULL, co_read, NULL) == -1) {
        printf("create read failed\n");
        exit(-1);
    }

    co_idle();

    return 0;
}
