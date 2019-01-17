

/* 在运行 top 命令时按下 “1” 键，可以看到每个 CPU 执行进程所占用的百分比。 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
//#define _GNU_SOURCE
#include <sched.h>

void wast_time() {
    int b = 1000000;
    int tmp = 0;
    while(0 < b--) {
        tmp = 10000*10000;
    }
    struct timeval val = {10, 1000};
    select(0, NULL, NULL, NULL, &val);
}

int main() {
    cpu_set_t mask;
    //cpu0
    sleep(20);
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    int retval = sched_setaffinity(0, sizeof(mask), &mask);
    if (retval < 0) {
        perror("sched_setaffinity:0");
    }
    wast_time();

    // cpu1
    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
    retval = sched_setaffinity(0, sizeof(mask), &mask);
    if (retval < 0) {
        perror("sched_setaffinity:1");
    }
    wast_time();

    // cpu2
    CPU_ZERO(&mask);
    CPU_SET(2, &mask);
    retval = sched_setaffinity(0, sizeof(mask), &mask);
    if (retval < 0) {
        perror("sched_setaffinity:1");
    }
    wast_time();

    // cpu 3
    CPU_ZERO(&mask);
    CPU_SET(3, &mask);
    retval = sched_setaffinity(0, sizeof(mask), &mask);
    if (retval < 0) {
        perror("sched_setaffinity:1");
    }
    wast_time();

    int procs_num = sysconf(_SC_NPROCESSORS_CONF);
    printf("procs_num: %d\n", procs_num);

    return 0;

}
