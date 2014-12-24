#ifndef __IO_H__
#define __IO_H__


#include <unistd.h>
#include "sched.h"


static inline ssize_t
co__read(int fd, void *buf, size_t count)
{
  ssize_t n, bytes;

  bytes = 0;
  for (;;) {
    if ((n = read(fd, ((char*)buf) + bytes, count - bytes)) == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        if (bytes != 0) {
          break;
        }
        coroutine_sched_block(g_coroutine_running_ctx, fd, EPOLLIN);
        continue;
      } else {
        bytes = -1;
        break;
      }
    }
    bytes += n;

    if (n == 0) {
      break;
    }
  }

  return nbytes;
}


ssize_t
co__write(int fd, const void *buf, size_t count)
{
  ssize_t n, bytes;

  bytes = 0;
  for (;;) {
    if ((n = write(fd, ((char*)buf) + bytes, count - bytes)) == -1) {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        coroutine_sched_block(g_coroutine_running_ctx, fd, EPOLLOUT);
        continue;
      } else {
        bytes = -1;
        break;
      }
    }
    bytes += n;

    if (bytes == (ssize_t)count) {
      break;
    }
  }

  return bytes;
}


#endif

