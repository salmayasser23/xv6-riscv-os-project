#include "kernel/types.h"
#include "user/user.h"

int main() {
  get_metrics();  // will print averages from kernel
  exit(0);
}
