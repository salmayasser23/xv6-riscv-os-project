#include "kernel/types.h"
#include "user/user.h"

void itoa(int val, char *buf) {
  int i = 0, j = 0;
  char tmp[16];
  if (val == 0) { buf[i++] = '0'; buf[i] = 0; return; }
  while (val > 0) { tmp[j++] = '0' + (val % 10); val /= 10; }
  while (j > 0) buf[i++] = tmp[--j];
  buf[i] = 0;
}

void atomic_print(int pid, const char *msg, int prio) {
  char buf[128], pidbuf[16], priobuf[16];
  int p = 0;
  buf[p++] = '[';
  char pre[] = "Child ";
  for (int i = 0; pre[i]; i++) buf[p++] = pre[i];
  itoa(pid, pidbuf);
  for (int i = 0; pidbuf[i]; i++) buf[p++] = pidbuf[i];
  buf[p++] = ']'; buf[p++] = ' ';
  for (int i = 0; msg[i]; i++) buf[p++] = msg[i];
  if (prio > 0) {
    char pr[] = " (priority=";
    for (int i = 0; pr[i]; i++) buf[p++] = pr[i];
    itoa(prio, priobuf);
    for (int i = 0; priobuf[i]; i++) buf[p++] = priobuf[i];
    buf[p++] = ')';
  }
  buf[p++] = '\n';
  write(1, buf, p);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: schedtest <num_processes> [<prio1> <prio2> ...]\n");
    exit(1);
  }

  int n = atoi(argv[1]);
  if (n <= 0 || n > 64) {
    printf("Invalid number of processes.\n");
    exit(1);
  }

  int sched_mode = getsched(); // 0 = RR, 1 = FCFS, 2 = Priority
  int priorities[64];

  if (sched_mode == 2) {
    if (argc != n + 2) {
      printf("Priority mode requires %d priorities\n", n);
      exit(1);
    }
    for (int i = 0; i < n; i++) {
      priorities[i] = atoi(argv[i + 2]);
      if (priorities[i] <= 0) {
        printf("Priorities must be positive integers\n");
        exit(1);
      }
    }
  }

  for (int i = 0; i < n; i++) {
    if (sched_mode == 1) sleep(2); // FCFS spacing

    int pid = fork();
    if (pid < 0) {
      printf("Fork failed\n");
      exit(1);
    }

    if (pid == 0) {
      // child waits to give time for parent to set priority
      sleep(5);

      int mypid = getpid();
      atomic_print(mypid, "started", (sched_mode == 2 ? priorities[i] : -1));

      for (volatile int j = 0; j < 100000000; j++);

      atomic_print(mypid, "done", -1);
      exit(0);
    } else {
      if (sched_mode == 2) {
        setpriority(pid, priorities[i]);
      }
    }
  }

  for (int i = 0; i < n; i++) wait(0);
  exit(0);
}
