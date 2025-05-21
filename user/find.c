#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// Recursively search for a file with a specific name
void find(const char *path, const char *target) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // Open the directory
  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  // Get file/directory info
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // Ensure it's a directory
  if (st.type != T_DIR) {
    close(fd);
    return;
  }

  // Prepare path buffer
  strcpy(buf, path);
  p = buf + strlen(buf);
  if (*(p - 1) != '/') {
    *p++ = '/';
  }

  // Read entries inside the directory
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) continue;
    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    // Get stat info for the entry
    if (stat(buf, &st) < 0) {
      fprintf(2, "find: cannot stat %s\n", buf);
      continue;
    }

    // If directory, search deeper
    if (st.type == T_DIR) {
      find(buf, target);
    } else {
      // If file matches, print path
      if (strcmp(de.name, target) == 0) {
        printf("%s\n", buf);
      }
    }
  }

  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc == 2 && strcmp(argv[1], "?") == 0) {
    printf("Usage: find path filename\n");
    exit(0);
  }

  if (argc != 3) {
    fprintf(2, "Usage: find path filename\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
