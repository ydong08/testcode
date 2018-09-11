#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>


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

    // return status
    int* pret = new int(loop);
    pthread_exit(pret);
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
        fflush(stdout);
        sleep(1);
    }

    void* ret;
    if(pthread_join(pid, &ret)){
        perror("join thread");
        abort();
    }

    printf("\nsub thread return: %d\n", *(int*)ret);
    delete (int*)ret;

    printf("\nloop equal %d\n", loop);
    // monotonic clock
    struct timespec monotonic_time;
    memset(&monotonic_time, 0x00, sizeof(struct timespec));
    syscall(SYS_clock_gettime, CLOCK_MONOTONIC_RAW, &monotonic_time);
    struct tm tie;
    memset(&tie, 0x00, sizeof(tie));

    return 0;
}


 










