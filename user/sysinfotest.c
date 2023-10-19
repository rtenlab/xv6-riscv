// Test that sysinfo returns:
//  - number of active process if provided 0.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(void)
{
  int n_active_proc, n_syscalls, n_free_pages;

  n_active_proc = sysinfo(0);
  n_syscalls = sysinfo(1);
  n_free_pages = sysinfo(2);
  printf("[sysinfo] active proc: %d, syscalls: %d, free pages: %d\n",
  n_active_proc, n_syscalls, n_free_pages);

  exit(0);
}
