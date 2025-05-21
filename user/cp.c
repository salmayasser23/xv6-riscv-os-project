#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

#define BUF_SIZE 512

int main(int argc, char *argv[]) {
  int src_fd, dest_fd, n;
  char buf[BUF_SIZE];

  // Help
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: cp source_file destination_file\n");
    exit(0);
  }

  // Argument check
  if (argc != 3) {
    fprintf(2, "Usage: cp source_file destination_file\n");
    exit(1);
  }

  // Open source file
  src_fd = open(argv[1], O_RDONLY);
  if (src_fd < 0) {
    fprintf(2, "cp: cannot open %s\n", argv[1]);
    exit(1);
  }

  // Create destination file
  dest_fd = open(argv[2], O_CREATE | O_WRONLY);
  if (dest_fd < 0) {
    fprintf(2, "cp: cannot create %s\n", argv[2]);
    close(src_fd);
    exit(1);
  }

  // Copy loop
  while ((n = read(src_fd, buf, sizeof(buf))) > 0) {
    if (write(dest_fd, buf, n) != n) {
      fprintf(2, "cp: write failed\n");
      close(src_fd);
      close(dest_fd);
      exit(1);
    }
  }

  // Cleanup
  close(src_fd);
  close(dest_fd);
  exit(0);
}
