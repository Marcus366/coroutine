#include <unistd.h>
#include <fcntl.h>
#include "coroutine.h"


int
co_pipe_open(int pipefd[2])
{
  return pipe2(pipefd, O_NONBLOCK);
}


int
co_pipe2_open(int pipefd[2], int flags)
{
  return pipe2(pipefd, flags | O_NONBLOCK);
}

