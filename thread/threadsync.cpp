#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>
#include <math.h>

/* KP1 ~ KP4 */

using namespace std;

int loop = 0;
void* addThread(void* p)
{
    int i=0,j=0;
    for(;i < 20; ++i){
        j = loop;
        j += 1;
        printf(".");
        fflush(stdout);
        sleep(1);
        loop = j;
    }

    // KP1: return status
#ifdef NUMBER
    pthread_exit(&loop);
#else
    pthread_exit(const_cast<char*>("loop"));
#endif
    
}


int main()
{
    pthread_t pid;
    if(0 != pthread_create(&pid, NULL, addThread, NULL)){
        perror("create thread");
        abort();
    }

    for(int i = 0; i < 20; ++i){
        loop += 1;
        printf("o");
        // KP2: 即时刷新缓存输出
        fflush(stdout); 
        sleep(1);
    }

    // KP3: 获取线程状态值
    void* ret;
    if(pthread_join(pid, &ret)){
        perror("join thread");
        abort();
    }
#ifdef NUMBER
    printf("\nsub thread return: %d\n", *(int*)ret);  // !!!
#else
    printf("\nsub thread return: %s\n", (char*)ret);
#endif
    //  KP4: monotonic clock 单调递增时间使用方法
    struct timespec monotonic_time; 
    // wall time, real time, from 1970, nsec
    // monotonic time, affect by ntp, sleep time, time from start, nsec
    // monotonic time raw, affect by sleep time, time from start, nsec
    // boot time, tatol time from start, nsec
    memset(&monotonic_time, 0x00, sizeof(struct timespec));
    syscall(SYS_clock_gettime, CLOCK_MONOTONIC_RAW, &monotonic_time);  // !!!
    printf("CurTimeSec: %Lf\n", monotonic_time.tv_sec + (monotonic_time.tv_nsec/powl(10, 9)));

    return 0;
}










