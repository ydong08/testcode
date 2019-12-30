#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
  const char* file = "tmp.log";
  FILE* fp = fopen(file, "w+");
  if (fp) {
    if (flock(fileno(fp), LOCK_EX) != -1) {
      printf("lock file success\n");
    }
  }

  fclose(fp);

  return 0;
}