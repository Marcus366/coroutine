#ifndef __UTILS_H__
#define __UTILS_H__


#include <sys/types.h>

int set_nonblocking(int fd);
int getfl(int fd);
int setfl(int fd, int fl);


int is_nonblocking(int fd);


#endif

