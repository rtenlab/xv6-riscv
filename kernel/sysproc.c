#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sysinfo(void)
{
  int param;
  argint(0, &param);

  if (param == 0) {
    return total_active_process_count();
  }else if (param == 1) {
    return get_total_num_syscalls();
  }else if (param == 2) {
    return num_free_pages();
  }
  
  return -1;
}

uint64
sys_procinfo(void)
{
  uint64 pinfo_ptr; // user pointer to pinfo struct to be filled.
  argaddr(0, &pinfo_ptr);

  if (pinfo_ptr == 0) {
    // User provided nullptr.
    return -1;
  }
  
  struct pinfo in;
  struct proc *p = myproc();

  // fills the pinfo information from current process.
  fill_pinfo(p, &in);

  // copy the pinfo struct back to user space.
  if(copyout(p->pagetable, pinfo_ptr, (char *)&in, sizeof(in)) < 0)
      return -1;
  return 0;
}
