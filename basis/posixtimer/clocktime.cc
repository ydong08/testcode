
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/*
内核中有多种timeline时间线，比较基础且有代表性的时间线有：
CLOCK_REALTIME：
UTC，又称墙上时间，内核中一般写作xtime，代表真实世界的时间，实际上保存的是距离1970-1-1 0时（linux epoch）的时间。
UTC时间其实并不是绝对准确的，它和原子钟之间是有偏差的，UTC在有些时候会加入闰秒来调整世界时。
CLOCK_MONOTONIC:
内核中单调递增的时间，不包含系统睡眠的时间。
其中保存的实际上是距离realtime的时间差，也就是说它的值要加上CLOCK_REALTIME的值才是monotonic时间。
这里可能会有一些疑问，比如修改了系统时间不就会对这个时间产生影响吗？
其实并不会，在修改系统时间的时候，会将设置时间和原系统时间的偏差加到CLOCK_MONOTONIC中，
这样就保证了通过CLOCK_MONOTONIC获得是时间是单调递增的。
CLOCK_BOOTTIME:
内核从boot启动后经过的时间，包含系统睡眠的时间，相当于CLOCK_MONOTONIC+睡眠时间。
CLOCK_MONOTONIC_RAW
绝对单调的时间，不受ntp系统的影响，不会微调.
CLOCK_TAI
原子钟时间，比UTC更严格。
CLOCK_PROCESS_CPUTIME_ID 的含义与 setitimer 的ITIMER_VIRTUAL类似。计时器只记录当前进程所实际花费的时间；
比如系统繁忙，当前进程只能获得 50%的CPU时间，为了让进程真正地运行10分钟，应该到10点30分才允许Timer到期。
CLOCK_THREAD_CPUTIME_ID 以线程为计时实体，当前进程中的某个线程真正地运行了一定时间才触发 Timer。

*/

int main() {

    // clock_gettime
    struct timespec ts;
    memset(&ts, 0x00, sizeof(ts));
    //1. CLOCK_REALTIME
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("clock realtime        : %ldus\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 1.1 CLOCK_REALTIME_COARSE
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_REALTIME_COARSE, &ts);
    printf("clock realtime coarse : %ldus\n\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 2. CLOCK_MONOTONIC
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("clock monotonic       : %ldus\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 2.1 CLOCK_MONTONIC_COARSE
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_MONOTONIC_COARSE, &ts);
    printf("clock monotonic coarse: %ldus\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 3. CLOCK_MONOTONIC_RAW
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    printf("clock monotonic raw   : %ldus\n\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 4. CLOCK_PROCEE_CPUTIME_ID
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    printf("clock process cputime : %ldus\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 5. CLOCK_THREAD_CPUTIME_ID
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
    printf("clock thread cputime  : %ldus\n\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 6. CLOCK_BOOTTIME
    memset(&ts, 0x00, sizeof(ts));
    clock_gettime(CLOCK_BOOTTIME, &ts);
    printf("clock bootime         : %ldus\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us

    // 7. CLOCK_TAI
    // memset(&ts, 0x00, sizeof(ts));
    // clock_gettime(CLOCK_TAI, &ts);
    // printf("clock tai             : %ldus\n", ts.tv_sec*1000000 + ts.tv_nsec/1000);  // us


    return 0;

}

