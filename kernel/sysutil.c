#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"




extern int kbd_intr_count;

uint64
sys_kbdint(void)
{
  return kbd_intr_count;
}


int syscall_count = 0;

uint64
sys_countsyscall(void)
{
    return syscall_count;
}


uint64
sys_getppid(void)
{
    struct proc *p = myproc();  // get current process
    if (p->parent)
        return p->parent->pid;
    return -1;  // no parent found
}


struct user_proc {
  int pid;
  int ppid;
  int state;
  uint64 sz;
  char name[16];
};

uint64
sys_getptable(void) {
  int max;
  uint64 buf;

  // Fetch syscall arguments by reference (no return value)
  argint(0, &max);
  argaddr(1, &buf);

  // Optional sanity check
  if (max <= 0 || max > NPROC)
      return -1;

  struct proc *p;
  struct user_proc up;
  int count = 0;

  for (p = proc; p < &proc[NPROC] && count < max; p++) {
      acquire(&p->lock);
      if (p->state != UNUSED) {
          up.pid = p->pid;
          up.ppid = p->parent ? p->parent->pid : 0;
          up.state = p->state;
          up.sz = p->sz;
          safestrcpy(up.name, p->name, sizeof(up.name));

          if (copyout(myproc()->pagetable, buf + count * sizeof(up),
                      (char *)&up, sizeof(up)) < 0) {
              release(&p->lock);
              return -1;
          }

          count++;
      }
      release(&p->lock);
  }

  return count;
}
