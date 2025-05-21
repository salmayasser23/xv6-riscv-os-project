#include "kernel/types.h"
#include "user/user.h"


int main() {
    struct user_proc procs[64];
    int n = getptable(64, procs);

    printf("PID\tPPID\tSTATE\tSIZE\tNAME\n");
    for(int i = 0; i < n; i++) {
      printf("%d\t%d\t%d\t%lu\t%s\n", procs[i].pid, procs[i].ppid, procs[i].state, procs[i].sz, procs[i].name);

    }

    exit(0);
}
