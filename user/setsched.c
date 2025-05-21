#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if(argc != 2) {
    printf("Usage: setsched <mode>\n");
    exit(1);
  }
  int mode = atoi(argv[1]);
  setsched(mode);
  exit(0);
}
