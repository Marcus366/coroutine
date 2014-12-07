#include <unistd.h>
#include <fcntl.h>
#include "utils.h"
#include "sched.h"


int
set_nonblocking(int fd)
{
    int fl;

    if ((fl = getfl(fd)) == -1) {
        return -1;
    }

    if (setfl(fd, fl | O_NONBLOCK) == -1) {
        return -1;
    }

    return 0;
}


int
is_nonblocking(int fd)
{
    return g_fds[fd].fl & O_NONBLOCK;
}


int
getfl(int fd)
{
    return fcntl(fd, F_GETFL);
}


int
setfl(int fd, int fl)
{
    if (fcntl(fd, F_SETFL, fl) != 0) {
        return -1;
    }

    return 0;
}

