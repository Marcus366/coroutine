#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "coroutine.h"


#define IGN      0
#define CR       1
#define ENDLINE  2
#define ALMOST   3
#define DONE     4

int uuid = 0;

const char *out = "HTTP/1.1 200 OK\r\n"
    "Server: COROUTINE/0.1\r\n"
    "Date: Sat, 31 Dec 2014 23:59:59\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 0\r\n"
    "\r\n";

void co_listen(void *arg)
{
  int i, n, fd, state;
  char buf[1024];

  state = 0;
  fd = (long)arg;
  while (state != DONE && (n = co_tcp_read(fd, buf, 1024)) > 0) {
    
      for (i = 0; i < n; ++i) {
          switch(state) {
          case IGN:
              if (buf[i] == '\r') {
                  state = CR;
              }
              break;
          case CR:
              if (buf[i] == '\n') {
                  state = ENDLINE;
              } else {
                  state = IGN;
              }
              break;
          case ENDLINE:
              if (buf[i] == '\r') {
                  state = ALMOST;
              } else {
                  state = IGN;
              }
              break;
          case ALMOST:
              if (buf[i] == '\n') {
                  state = DONE;
              } else {
                  state = IGN;
              }
              break;
          case DONE:
          default:
              break;
          }
      }

  }

  if (state == DONE) {
      (void) co_write(fd, out, strlen(out));
      (void) close(fd);
  }

  return ;
}


int main()
{
    int listenfd;

    crt_init();

    if ((listenfd = co_tcp4_open_bind(NULL, 80)) == -1) {
        perror("open bind error");
        exit(-1);
    }

    if (co_tcp_listen(listenfd, 1024) == -1) {
      perror("listen error");
      exit(-1);
    }

    for (;;) {
      int connfd;

      if ((connfd = co_tcp_accept(listenfd, NULL, NULL)) == -1) {
        perror("accept error");
        exit(-1);
      }

      if (crt_create(NULL, co_listen, (void*)(uint64_t)connfd) == NULL) {
        printf("crt_create error");
        exit(-1);
      }
    }

    return 0;
}

