#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: fact number\n");
    exit(0);
  }

  if (argc != 2) {
    fprintf(2, "You can only get the factorial of a single number\n");
    exit(1);
  }

  int n = atoi(argv[1]);
  if (n < 0) {
    fprintf(2, "Error: Negative number not allowed.\n");
    exit(1);
  }

  int result = 1;
  for (int i = 2; i <= n; i++) {
    result *= i;
  }

  printf("%d\n", result);
  exit(0);
}
