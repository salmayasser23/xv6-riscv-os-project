#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "datetime.h"

#define MTIME_FREQ 10000000  // for 10 MHz timer (typical in QEMU)




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
  if(n < 0)
    n = 0;
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
sys_datetime(void)
{
  uint64 dst;                 // User pointer where datetime struct should be copied
  struct datetime dt;

  // Retrieve the pointer from the user program (1st argument)
  argaddr(0, &dst);

  // Get current time in ticks from the mtime register
  uint64 seconds = r_mtime() / MTIME_FREQ;
  seconds += BOOT_EPOCH;
  seconds += 3 * 60 * 60; // add offset for UTC+3

  // Convert UNIX timestamp to datetime
  epoch_to_datetime(seconds, &dt);

  // Copy the datetime struct to user space
  if (copyout(myproc()->pagetable, dst, (char *)&dt, sizeof(dt)) < 0)
    return -1;

  return 0;
}

uint64 sys_krand(void) {
  static uint seed = 1;
  seed = seed * 1664525 + 1013904223;
  return seed;
}

extern int sched_mode;
uint64
sys_setsched(void) {
  int n;
  argint(0, &n);
  if(n < 0)
    return -1;
  printf(">> sys_setsched: changing sched_mode %d → %d\n",sched_mode, n);
  sched_mode = n;
  return 0;
}

extern int sched_mode;
uint64 sys_getsched(void) {
  return sched_mode;
}


uint64 sys_setpriority(void)
{
  int pid, priority;
  argint(0, &pid);
  argint(1, &priority);
  if (pid < 0 || priority < 0)
    return -1;

  printf(">> sys_setpriority: pid=%d → new priority=%d\n", pid, priority);

  for (struct proc *p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);  // acquire lock before modifying
    if (p->pid == pid) {
      p->priority = priority;
      release(&p->lock);
      return 0;
    }
    release(&p->lock);
  }
  return -1;
}
