#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void cleanup(void* p)
{
    printf("cleanup cancel\n");
}

void* thread(void* p) {
    
    int count = 100;
    pthread_cleanup_push(cleanup, NULL);
    do
    {
        printf("%02d\n", count);
        usleep(1000000);
    } while (0 < count--);

    printf("cleanup exit\n");
    pthread_cleanup_pop(1);
    pthread_exit((char*)2);
}

void* cenceltherad(void* p)
{
    pthread_t *tid = (pthread_t*)p;

    usleep(10000000);
    pthread_cancel(*tid);

    pthread_exit((char*)1);

}

int main() {
    pthread_t tid, tid2;
    int retval = pthread_create(&tid, NULL, thread, NULL);
    if (retval)
        perror("pthread_create");

#if 0
    retval = pthread_create(&tid2, NULL, cenceltherad, &tid);
    if (retval)
        perror("pthread_create cancel");
#endif
    
    void* st ;

    pthread_join(tid, (void**)&st);
    printf("tid st: %d\n", st);

    pthread_join(tid2, (void**)&st);
    printf("tid2 st: %d\n", st);

    return 0;

}




