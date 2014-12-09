#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../src/coroutine.h"


#define IGN      0
#define CR       1
#define ENDLINE  2
#define ALMOST   3
#define DONE     4


const char *out = "HTTP/1.1 200 OK\r\n"
    "Server: COROUTINE/0.1\r\n"
    "Date: Sat, 31 Dec 2014 23:59:59\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 0\r\n"
    "\r\n";

void* co_listen(void *arg)
{
  int i, n, fd, state;
  char buf[1024];

  state = 0;
  fd = (long)arg;

  while ((n = read(fd, buf, 1024)) > 0) {
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
      ++i;
    }
  }

  if (state == DONE) {
    write(fd, out, strlen(out));
  }

  return NULL;
}


int main()
{
  int sockfd;
  struct sockaddr_in addr;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(8080);

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind error");
    exit(-1);
  }

  if (listen(sockfd, 1024) == -1) {
    perror("listen error");
    exit(-1);
  }

  for (;;) {
    int connfd;
    coroutine_t cid;
    socklen_t socklen;
    struct sockaddr_in cliaddr;

    socklen = sizeof(cliaddr);
    if ((connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &socklen)) == -1) {
      perror("accept error");
      exit(-1);
    }

    if (coroutine_create(&cid, NULL, co_listen, (void*)connfd) == -1) {
      printf("coroutine_create error");
      exit(-1);
    }
  }
  return 0;
}

