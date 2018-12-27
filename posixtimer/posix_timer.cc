
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main() {

    // clock_gettime
    struct timespec ts;
    memset(&ts, 0x00, sizeof(ts));
    //1. CLOCK_REALTIME
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("clock realtime      : %ld\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 1.1 CLOCK_REALTIME_COARSE
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_REALTIME_COARSE, &ts);
    printf("clock realtime coarse: %ld\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 2. CLOCK_MONOTONIC
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("clock monotonic      : %ld\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 2.1 CLOCK_MONTONIC_COARSE
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
    printf("clock monotonic coarse: %ld\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 3. CLOCK_MONOTONIC_RAW
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    printf("clock monotonic raw   : %ld\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 4. CLOCK_PROCEE_CPUTIME_ID
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    printf("clock process cputime : %ld\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 5. CLOCK_THREAD_CPUTIME_ID
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    printf("clock thread cputime  : %ld\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 6. CLOCK_BOOTTIME
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_BOOTTIME, &ts);
    printf("clock bootime        : %ld\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    return 0;

}

