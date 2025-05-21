#include "kernel/types.h"
#include "kernel/datetime.h"
#include "user/user.h"

int main() {
  struct datetime dt;  // Stack-allocated variable

  if (datetime(&dt) < 0) {
    printf("datetime syscall failed\n");
    exit(1);
  }

  printf("Date: %d-%d-%d\n", dt.year, dt.month, dt.day);
  printf("Time: %d:%d:%d\n", dt.hour, dt.minute, dt.second);


  exit(0);
}
