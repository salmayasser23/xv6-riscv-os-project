// user/pr.c
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
  if(argc != 3){
    printf("usage: pr <pid> <priority>\n");
    exit(1);
  }
  int pid  = atoi(argv[1]);
  int prio = atoi(argv[2]);
  if(setpriority(pid, prio) < 0){
    printf("pr: setpriority(%d,%d) failed\n", pid, prio);
    exit(1);
  }
  exit(0);
}
