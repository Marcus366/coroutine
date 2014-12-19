#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <errno.h>

#include "utils.h"
#include "sched.h"
#include "coroutine.h"


#define syscall1(name, ret, arg1)                         \
  typedef ret (*g_##name##_ptr_t)(arg1);                  \
g_##name##_ptr_t g_##name##_ptr = NULL

#define syscall2(name, ret, arg1, arg2)                   \
  typedef ret (*g_##name##_ptr_t)(arg1, arg2);            \
g_##name##_ptr_t g_##name##_ptr = NULL

#define syscall3(name, ret, arg1, arg2, arg3)             \
  typedef ret (*g_##name##_ptr_t)(arg1, arg2, arg3);      \
g_##name##_ptr_t g_##name##_ptr = NULL

#define syscall4(name, ret, arg1, arg2, arg3, arg4)       \
  typedef ret (*g_##name##_ptr_t)(arg1, arg2, arg3, arg4);\
g_##name##_ptr_t g_##name##_ptr = NULL


#define hook_sys_call(name)                               \
  do {                                                    \
    if (g_##name##_ptr == NULL) {                         \
      g_##name##_ptr =                                    \
      (g_##name##_ptr_t)dlsym(RTLD_NEXT, #name);          \
    }                                                     \
  } while (0)


typedef  __socklen_t socklen_t;


syscall1(close, int, int);

syscall3(open, int, const char*, int, mode_t);
syscall2(creat, int, const char*, mode_t);
syscall3(socket, int, int, int, int);
syscall3(accept, int, int, struct sockaddr *addr, socklen_t *addrlen);

syscall3(fcntl, int, int, int, ...);

syscall3(read, ssize_t, int, void*, size_t);
syscall3(write, ssize_t, int, const void*, size_t);

syscall4(recv, ssize_t, int, void*, size_t, int);
syscall4(send, ssize_t, int, const void*, size_t, int);


int
close(int fd)
{
  hook_sys_call(close);

  coroutine_sched_unregfd(fd);

  return g_close_ptr(fd);
}


int
open(const char *pathname, int flags, mode_t mode)
{
  int fd;
  hook_sys_call(open);

  fd = g_open_ptr(pathname, flags, mode);

  if (fd == -1) {
    return -1;
  }

  if (coroutine_sched_regfd(fd, flags) == -1) {
    close(fd);
    return -1;
  }

  return fd;
}


int
creat(const char *pathname, mode_t mode)
{
  int fd;
  hook_sys_call(creat);

  fd = g_creat_ptr(pathname, mode);

  if (fd == -1) {
    return -1;
  }

  if (coroutine_sched_regfd(fd, 0) == -1) {
    close(fd);
    return -1;
  }

  return fd;
}


int
socket(int domain, int type, int protocol)
{
  int fd;
  hook_sys_call(socket);

  fd = g_socket_ptr(domain, type, protocol);

  if (fd == -1) {
    return -1;
  }

  if (coroutine_sched_regfd(fd, type) == -1) {
    close(fd);
    return -1;
  }

  return fd;
}


int
accept(int socket, struct sockaddr *addr, socklen_t *addrlen)
{
  int connfd;
  hook_sys_call(accept);

  if (is_nonblocking(socket)) {
    return g_accept_ptr(socket, addr, addrlen);
  }

loop:
  if ((connfd = g_accept_ptr(socket, addr, addrlen)) == -1) {
    if (errno == EINTR) {
      goto loop;
    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
      coroutine_sched_block(g_coroutine_running_ctx, socket, EPOLLIN);
      goto loop;
    } else {
      connfd = -1;
    }
  }

  if (connfd != -1) {
    coroutine_sched_regfd(connfd, 0);
  }

  return connfd;
}


/*
 * If the fd state has been set nonblock by user, we have no need to hook it.
 *
 * Otherwise, we use nonblocking read instead of blocking read and if the read would
 * block the process, we should schedule this coroutine.
 */
ssize_t
read(int fd, void *buf, size_t count)
{
  ssize_t n, bytes;
  hook_sys_call(read);
  //printf("hook read\n");

  if (is_nonblocking(fd)) {
    return g_read_ptr(fd, buf, count);
  }

  bytes = 0;
  for (;;) {
    if ((n = g_read_ptr(fd, ((char*)buf) + bytes, count - bytes)) == -1) {
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

  return bytes;
}


ssize_t
write(int fd, const void *buf, size_t count)
{
  ssize_t n, bytes;
  hook_sys_call(write);
  //printf("hook write\n");

  if (is_nonblocking(fd)) {
    return g_write_ptr(fd, buf, count);
  }

  bytes = 0;
  for (;;) {
    if ((n = g_write_ptr(fd, ((char*)buf) + bytes, count - bytes)) == -1) {
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


ssize_t
recv(int socket, void *buf, size_t len, int flags)
{
  ssize_t n, bytes;
  hook_sys_call(recv);
  //printf("hook recv\n");

  if (is_nonblocking(socket)) {
    return g_recv_ptr(socket, buf, len, flags);
  }

  bytes = 0;
  for (;;) {
    if ((n = g_recv_ptr(socket, ((char*)buf) + bytes,
            len - bytes, flags)) == -1)
    {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        coroutine_sched_block(g_coroutine_running_ctx, socket, EPOLLIN);
        continue;
      } else {
        bytes = -1;
        break;
      }
    }
    bytes += n;

    if (n == 0 || bytes == (ssize_t)len) {
      break;
    }
  }

  return bytes;
  //printf("hook recv\n");

  return g_recv_ptr(socket, buf, len, flags);
}


ssize_t
send(int socket, void *buf, size_t len, int flags)
{
  ssize_t n, bytes;
  hook_sys_call(send);
  //printf("hook send\n");

  if (is_nonblocking(socket)) {
    return g_send_ptr(socket, buf, len, flags);
  }

  bytes = 0;
  for (;;) {
    if ((n = g_send_ptr(socket, ((char*)buf) + bytes,
            len - bytes, flags)) == -1)
    {
      if (errno == EINTR) {
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        coroutine_sched_block(g_coroutine_running_ctx, socket, EPOLLOUT);
        continue;
      } else {
        bytes = -1;
        break;
      }
    }
    bytes += n;

    if (bytes == (ssize_t)len) {
      break;
    }
  }

  return bytes;
}

