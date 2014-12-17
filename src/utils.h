#ifndef __UTILS_H__
#define __UTILS_H__


#include <sys/types.h>

#include "global.h"
#include "sched.h"


/**
 * Set the state of given file descriptor as nonblocking.
 * @fd: the fd is going to be set nonblocking.
 *
 * @return: 0 stands for success while -1 stands for error.
 */
int set_nonblocking(int fd);


int getfl(int fd);
int setfl(int fd, int fl);


int is_nonblocking(int fd);


#endif

