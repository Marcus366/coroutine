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

int uuid = 0;

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

  while (state != DONE && (n = read(fd, buf, 1024)) > 0) {
    //buf[n] = 0;
    //printf("read %s\n", buf);
    
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
    (void) write(fd, out, strlen(out));
    (void) close(fd);
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
    socklen_t socklen;
    struct sockaddr_in cliaddr;

    socklen = sizeof(cliaddr);
    if ((connfd = accept(sockfd, (struct sockaddr*)&cliaddr, &socklen)) == -1) {
      perror("accept error");
      exit(-1);
    }

    if (++uuid == 1000 * 60) {
      return 0;
    }

    if (coroutine_create(NULL, co_listen, (void*)connfd) == NULL) {
      printf("coroutine_create error");
      exit(-1);
    }
  }
  return 0;
}


/*
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <event.h>


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
    "\r\n\r\n";

int uuid;

int
set_nonblocking(int fd)
{
  int fl;

  if ((fl = fcntl(fd, F_GETFL)) == -1) {
    return -1;
  }

  if (fcntl(fd, F_SETFL, fl | O_NONBLOCK) == -1) {
    return -1;
  }

  return 0;
}


typedef struct request_ctx_s {
  char *buf;
  int  size, pos, state;
  struct event *ev;
} request_ctx_t;


void
ev_write(int fd, short event, void *arg)
{
  int n;
  (void) event;
  request_ctx_t *ctx = (request_ctx_t*)arg;

  for (;;) {
    if ((n = write(fd, ctx->buf + ctx->pos, ctx->size - ctx->pos)) == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      } else {
        perror("read error");
        exit(-1);
      }
    }

    ctx->pos += n;

    if (n == 0) {
      break;
    }
  }

  if (n != 0) {
    event_add(ctx->ev, NULL);
  } else {
    free(ctx);
    close(fd);
  }
}


void
ev_read(int fd, short event, void *arg)
{
  int n, pos;
  (void) event;
  request_ctx_t *ctx = (request_ctx_t*)arg;
  pos = ctx->pos;

  // read request
  for (;;) {
    if ((n = read(fd, ctx->buf + ctx->pos, ctx->size - ctx->pos)) == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      } else {
        perror("read error");
        exit(-1);
      }
    }

    ctx->pos += n;

    if (n == 0) {
      break;
    }
  }

  // parse request
  for (n = pos; n < ctx->pos; ++n) {
    char c = ctx->buf[n];
    switch(ctx->state) {
    case IGN:
      if (c == '\r') {
        ctx->state = CR;
      }
      break;
    case CR:
      if (c == '\n') {
        ctx->state = ENDLINE;
      } else {
        ctx->state = IGN;
      }
      break;
    case ENDLINE:
      if (c == '\r') {
        ctx->state = ALMOST;
      } else {
        ctx->state = IGN;
      }
      break;
    case ALMOST:
      if (c == '\n') {
        ctx->state = DONE;
      } else {
        ctx->state = IGN;
      }
      break;
    case DONE:
    default:
      break;
    }
  }

  if (ctx->state == DONE) {
    free(ctx->buf);
    ctx->buf = (char*)out;
    ctx->pos = 0;
    ctx->size = strlen(out);
    event_set(ctx->ev, fd, EV_WRITE, ev_write, ctx);
  }

  event_add(ctx->ev, NULL);
}


void
ev_accept(int fd, short event, void *arg)
{
  (void) event;
  int connfd;
  socklen_t clisocklen;
  struct sockaddr_in cliaddr;

  for (;;) {
    clisocklen = sizeof(cliaddr);
    if ((connfd = accept(fd, (struct sockaddr*)&cliaddr, &clisocklen)) == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        break;
      } else {
        perror("read error");
        exit(-1);
      }
    }

    if (++uuid == 60000) {
      exit(0);
    }

    if (set_nonblocking(connfd) == -1) {
      printf("set nonblock error");
      exit(-1);
    }

    request_ctx_t *ctx = (request_ctx_t*)malloc(sizeof(request_ctx_t));
    ctx->ev = (struct event*)malloc(sizeof(struct event));
    ctx->buf = (char*)malloc(1024);
    ctx->pos = 0;
    ctx->size = 1024;
    ctx->state = IGN;

    event_set(ctx->ev, connfd, EV_READ, ev_read, ctx);
    event_add(ctx->ev, NULL);
  }

  event_add((struct event*)arg, NULL);
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

  if (set_nonblocking(sockfd) == -1) {
    printf("set nonblocking error");
    exit(-1);
  }

  struct event ev;
  event_init();
  event_set(&ev, sockfd, EV_READ, ev_accept, &ev);
  event_add(&ev, NULL);

  event_dispatch();

  return 0;
}
*/

