
#include <unistd.h>
#include <sys/select.h>

#define MAX_LOOP_NUM        10000

int main() {
  timeval ts;
  for (int i = 0; i < MAX_LOOP_NUM; ++i) {
    ts.tv_sec = 0;
    ts.tv_usec = 1000;
    select(0, NULL,NULL, NULL, &ts);
  }
  return 0;
}

