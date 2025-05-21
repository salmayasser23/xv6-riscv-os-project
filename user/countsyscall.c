#include "kernel/types.h"
#include "user/user.h"

int main() {
    int count = countsyscall();
    printf("System calls made since boot: %d\n", count);
    exit(0);
}
