#include "kernel/types.h"
#include "user/user.h"

int main() {
  int count = kbdint();
  printf("Keyboard interrupts since boot: %d\n", count);

  exit(0);
}
