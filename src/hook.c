#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>

#include "utils.h"
#include "sched.h"

#define syscall1(name, ret, arg1)                           \
    typedef ret (*g_##name##_ptr_t)(arg1);                  \
    g_##name##_ptr_t g_##name##_ptr = NULL

#define syscall2(name, ret, arg1, arg2)                     \
    typedef ret (*g_##name##_ptr_t)(arg1, arg2);            \
    g_##name##_ptr_t g_##name##_ptr = NULL

#define syscall3(name, ret, arg1, arg2, arg3)               \
    typedef ret (*g_##name##_ptr_t)(arg1, arg2, arg3);      \
    g_##name##_ptr_t g_##name##_ptr = NULL

#define syscall4(name, ret, arg1, arg2, arg3, arg4)         \
    typedef ret (*g_##name##_ptr_t)(arg1, arg2, arg3, arg4);\
    g_##name##_ptr_t g_##name##_ptr = NULL


#define hook_sys_call(name)                                 \
    do {                                                    \
        if (g_##name##_ptr == NULL) {                       \
            g_##name##_ptr =                                \
            (g_##name##_ptr_t)dlsym(RTLD_NEXT, #name);      \
        }                                                   \
    } while (0)


syscall1(close, int, int);

syscall3(open, int, const char*, int, mode_t);
syscall2(creat, int, const char*, mode_t);
syscall3(socket, int, int, int, int);

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

    if (coroutine_sched_regfd(fd) == -1) {
        close(fd);
        return -1;
    }

    return fd;
}


ssize_t
read(int fd, void *buf, size_t count)
{
    hook_sys_call(read);

    printf("hook read\n");

    return g_read_ptr(fd, buf, count);
}


ssize_t
write(int fd, const void *buf, size_t count)
{
    hook_sys_call(write);

    printf("hook write\n");

    return g_write_ptr(fd, buf, count);
}


ssize_t
recv(int socket, void *buf, size_t len, int flags)
{
    hook_sys_call(recv);

    printf("hook recv\n");

    return g_recv_ptr(socket, buf, len, flags);
}


ssize_t
send(int socket, void *buf, size_t len, int flags)
{
    hook_sys_call(send);

    printf("hook send\n");

    return g_send_ptr(socket, buf, len, flags);
}

