#include "kernel/types.h"
#include "user/user.h"

int main() {
    int pid=getpid();
    int ppid = getppid();

    printf("Process PID: %d\n", pid);
    printf("Parent PID: %d\n", ppid);
    exit(0);
}
