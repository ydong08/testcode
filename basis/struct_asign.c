#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

/* C语言的结构体是可以直接赋值的 */
void *mytest(void *arg)
{
    pthread_detach(pthread_self());

    time_t current;
    time(&current);

    struct tm *ptm = (struct tm*)arg;
    struct tm *pm = localtime(&current);
    *ptm = *pm;

    return NULL;
}

int main()
{
    pthread_t tid;
    struct tm tmv;
    memset(&tmv, 0x00, sizeof(tmv));
    pthread_create(&tid, NULL, mytest, &tmv);
    sleep(1);
    printf("CurTime: %04d-%02d-%02d-%02d:%02d:%02d\n", 
        tmv.tm_year+1900, tmv.tm_mon+1, tmv.tm_mday, tmv.tm_hour, tmv.tm_min, tmv.tm_sec);

}