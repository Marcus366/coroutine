#ifndef __IO_H__
#define __IO_H__


#include <unistd.h>
#include "sched.h"


static inline ssize_t
co__read(int fd, void *buf, size_t count)
{
  ssize_t n, nbytes;

  nbytes = 0;
  for (;;) {
    if ((n = read(fd, ((char*)buf) + nbytes, count - nbytes)) == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        if (nbytes != 0) {
          break;
        }
        coroutine_block(g_coroutine_running_ctx, fd, EPOLLIN);
        continue;
      } else {
        nbytes = -1;
        break;
      }
    }
    nbytes += n;

    if (n == 0) {
      break;
    }
  }

  return nbytes;
}


static inline ssize_t
co__write(int fd, const void *buf, size_t count)
{
  ssize_t n, nbytes;

  nbytes = 0;
  for (;;) {
    if ((n = write(fd, ((char*)buf) + nbytes, count - nbytes)) == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        coroutine_block(g_coroutine_running_ctx, fd, EPOLLOUT);
        continue;
      } else {
        nbytes = -1;
        break;
      }
    }
    nbytes += n;

    if (nbytes == (ssize_t)count) {
      break;
    }
  }

  return nbytes;
}


#endif

