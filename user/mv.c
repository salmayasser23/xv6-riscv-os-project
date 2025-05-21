#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char *argv[]) {
  // Help message
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: mv source_file destination_file\n");
    exit(0);
  }

  // Check for exactly 2 arguments
  if (argc != 3) {
    fprintf(2, "Usage: mv source_file destination_file\n");
    exit(1);
  }

  // Try to create a new hard link
  if (link(argv[1], argv[2]) < 0) {
    fprintf(2, "mv: failed to link %s to %s\n", argv[1], argv[2]);
    exit(1);
  }

  // Remove the old file
  if (unlink(argv[1]) < 0) {
    fprintf(2, "mv: failed to remove original file %s\n", argv[1]);
    exit(1);
  }

  exit(0);
}
