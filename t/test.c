#include "test.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


/* declare all the test */
#define MARCOS(name) TEST_DECLARE(name);
#include "test_list.h"
#undef MARCOS


#define MARCOS(name) {test_##name, 0},
static test_entry_t entries[] =
{
#include "test_list.h"
  {NULL, 0}
};
#undef MARCOS


int main()
{
  pid_t pid;
  int status, isPass = 1;
  test_entry_t *entry;

  for (entry = entries; entry->func; ++entry) {
    if ((pid = fork()) == 0) {
      return entry->func(entry->log);
    } else if (pid > 0) {
      (void) waitpid(pid, &status, 0);
      entry->code = WEXITSTATUS(status);
    } else {
      perror("test program fail");
    }
  }

  for (entry = entries; entry->func; ++entry) {
    if (entry->code != 0) {
      isPass = 0;
      printf("%s\n", entry->log);
    }
  }

  if (isPass) {
    printf("pass all test case\n");
  }

  return 0;
}
