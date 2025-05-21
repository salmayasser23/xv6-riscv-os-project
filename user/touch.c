#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
  int fd;
  struct stat st;

  // Help feature
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: touch filename\n");
    exit(0);
  }

  // Check for correct argument count
  if (argc != 2) {
    fprintf(2, "Usage: touch filename\n");
    exit(1);
  }

  // Check if file already exists
  if (stat(argv[1], &st) >= 0) {
    fprintf(2, "touch: file already exists: %s\n", argv[1]);
    exit(1);
  }

  // Create the file
  fd = open(argv[1], O_CREATE | O_WRONLY);
  if (fd < 0) {
    fprintf(2, "touch: cannot create %s\n", argv[1]);
    exit(1);
  }

  close(fd);
  exit(0);
}
