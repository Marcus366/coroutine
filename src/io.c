#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdarg.h>
#include <fcntl.h>
#include "sched.h"


int
co_file_open(const char *pathname, int flag, ... /* mode_t */)
{
    va_list ap;
    mode_t mode;

    if (flag & O_CREAT) {
        va_start(ap, flag);
        mode = va_arg(ap, mode_t);
        va_end(ap);

        return open(pathname, flag | O_NONBLOCK, mode);
    } else {
        return open(pathname, flag | O_NONBLOCK);
    }
}


ssize_t
co_read(int fd, void *buf, size_t count)
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
                crt_block(fd, EPOLLIN);
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


ssize_t
co_write(int fd, const void *buf, size_t count)
{
    ssize_t n, nbytes;

    nbytes = 0;
    for (;;) {
        if ((n = write(fd, ((char*)buf) + nbytes, count - nbytes)) == -1) {
            if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                crt_block(fd, EPOLLOUT);
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


ssize_t
co_pread(int fd, void *buf, size_t count, off_t offset)
{
    ssize_t n, nbytes;

    nbytes = 0;
    for (;;) {
        if ((n = pread(fd, ((char*)buf) + nbytes, count - nbytes, offset + nbytes)) == -1) {
            if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                if (nbytes != 0) {
                    break;
                }
                crt_block(fd, EPOLLIN);
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


ssize_t
co_pwrite(int fd, void *buf, size_t count, off_t offset)
{
    ssize_t n, nbytes;

    nbytes = 0;
    for (;;) {
        if ((n = pwrite(fd, ((char*)buf) + nbytes, count - nbytes, offset + nbytes)) == -1) {
            if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                crt_block(fd, EPOLLOUT);
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

