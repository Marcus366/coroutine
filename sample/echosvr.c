#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "coroutine.h"


static void
echo(void *arg)
{
    int fd;
    size_t size;
    char buf[1024];

    fd = (int)arg;
    while ((size = co_tcp_read(fd, buf, 1023)) > 0) {
        co_tcp_write(fd, buf, size);
    }

    close(fd);
    return ;
}

int
main(int argc, char *argv[])
{
    int listenfd, connfd;
    crt_ctx_t *ctx;

    if (argc != 2) {
        printf("%s usage: echosvr <port>\n", argv[0]);
        return 0;
    }

    crt_init();

    if ((listenfd = co_tcp4_open_bind(NULL, atoi(argv[1]))) == -1) {
        perror("co_tcp4_open_bind fail");
        exit(-1);
    }

    if (co_tcp_listen(listenfd, 1024) == -1) {
        perror("co_tcp_listen fail");
        exit(-1);
    }

    for (;;) {
        connfd = co_tcp_accept(listenfd, NULL, NULL);
        ctx = crt_create(NULL, echo, (void*)(int64_t)connfd);
        (void) ctx;
    }

    return 0;
}
