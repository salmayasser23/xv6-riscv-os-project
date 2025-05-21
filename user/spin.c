// user/spin.c
#include "kernel/types.h"
#include "user/user.h"

// tiny itoa (decimal)
static int itoa(int v, char *b) {
  int i = 0;
  if (v == 0) { b[i++] = '0'; b[i] = 0; return 1; }
  if (v < 0) { b[i++] = '-'; v = -v; }
  char t[12]; int ti = 0;
  while (v) { t[ti++] = '0' + (v % 10); v /= 10; }
  while (ti--)   b[i++] = t[ti];
  b[i] = 0;
  return i;
}

// Build and write one whole line at once:
static void atomic_line(int pid, const char *msg) {
  char buf[64]; int p = 0;
  // "spin "
  for (const char *s="spin "; *s; s++) buf[p++] = *s;
  // pid
  p += itoa(pid, buf + p);
  // ": "
  buf[p++] = ':'; buf[p++] = ' ';
  // message + '\n'
  for (const char *s=msg; *s; s++) buf[p++] = *s;
  buf[p++] = '\n';
  write(1, buf, p);    // <--- one atomic syscall
}

int main(int argc, char *argv[]) {
  int pid = getpid();
  atomic_line(pid, "started");

  // Check current scheduler mode
  int mode = getsched(); // 0 = RR, 1 = FCFS, 2 = Priority
  int delay = 500;


   // Set priority from command-line argument
   if (mode == 2 && argc == 2) {
    int prio = atoi(argv[1]);
    setpriority(pid, prio);
  }


  if (mode == 0 || mode == 2) {
    int now = uptime();
    int wake = ((now / delay) + 1) * delay;
    int diff = wake - now;
    sleep(diff); // synchronize wake-up
  }

  if(mode==1){
    sleep(300);
  }

  // Busy work
  for (volatile uint64 i = 0; i < 1000000000; i++) { }

  atomic_line(pid, "done");
  exit(0);
}
