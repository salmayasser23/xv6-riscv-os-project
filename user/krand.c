#include "kernel/types.h"
#include "user/user.h"

int main() {
  int r = krand();
  printf("krand() = %d\n", r);
  exit(0);
}
