#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc , char *argv[]){
  int n=atoi(argv[1]);
  sleep(n);
  exit(0);
}
