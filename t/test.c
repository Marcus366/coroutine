#include "test.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


/* declare all the test */
#define MARCOS(name) TEST_DECLARE(name);
#include "test_list.h"
#undef MARCOS


#define MARCOS(name) {test_##name, #name, 0},
static test_entry_t entries[] =
{
#include "test_list.h"
  {NULL, NULL, 0}
};
#undef MARCOS


ssize_t __write__(int fd, test_entry_t *entry);
ssize_t __read__(int fd, test_entry_t *entry);


int main()
{
  pid_t pid;
  int fd[2], status, isPass = 1;
  test_entry_t *entry;

  if (pipe(fd) == -1) {
    perror("create pipe fail");
    exit(EXIT_FAILURE);
  }

  for (entry = entries; entry->func; ++entry) {
    if ((pid = fork()) == 0) {
      entry->code = entry->func(entry->log);
      if (entry->code) {
        __write__(fd[0], entry);
      }
      return entry->code;
    } else if (pid > 0) {
      (void) waitpid(pid, &status, 0);
      entry->code = WEXITSTATUS(status);
      if (entry->code) {
        __read__(fd[0], entry);
      }
    } else {
      perror("test program fail");
    }
  }

  for (entry = entries; entry->func; ++entry) {
    if (entry->code != 0) {
      isPass = 0;
      printf("%s\n", entry->log);
    } else {
      printf("pass test %s\n", entry->name);
    }
  }

  if (isPass) {
    printf("pass all test case\n");
  } else {
    printf("some test cases fail\n");
  }

  return 0;
}


ssize_t __write__(int fd, test_entry_t *entry) {
  ssize_t nbytes, nwrite = 0;;
  size_t count = strlen(entry->log);

  while ((nbytes = write(fd, entry->log, count - nwrite)) > 0) {
    nwrite += nbytes;
  }

  return nwrite;
}


ssize_t __read__(int fd, test_entry_t *entry) {
  ssize_t nbytes, nread = 0;;
  size_t count = sizeof(entry->log);

  while ((nbytes = read(fd, entry->log, count - nread)) > 0) {
    nread += nbytes;
  }

  return nread;
}
