#include "kernel/types.h"
#include "user/user.h"

int main() {
  // Wait until all children have exited
  while (wait(0) > 0)
    ;

  exit(0);
}
