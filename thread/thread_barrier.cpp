
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#if 1
void* task1(void* p) {
    pthread_barrier_t* bat = (pthread_barrier_t*)p;
    printf("task1 will barrier block\n");
    pthread_barrier_wait(bat);
    printf("task1 will continue\n");
    pthread_exit(NULL);
}


void* task2(void* p) {
    pthread_barrier_t* bat = (pthread_barrier_t*)p;
    printf("task2 will barrier block\n");
    pthread_barrier_wait(bat);
    printf("task2 will continue\n");
    pthread_exit(NULL);
}

int main() {

    pthread_t tid1, tid2;
    pthread_attr_t attr;
    pthread_barrier_t bat;
    pthread_barrierattr_t battr;

    pthread_attr_init(&attr);
    pthread_barrierattr_init(&battr);
    pthread_barrier_init(&bat, &battr, 2+1);
    int retval = pthread_create(&tid1, &attr, task1, (void*)&bat);
    if (retval) {
        perror("task1 thread create");
        exit(1);
    }

    retval = pthread_create(&tid2, &attr, task2, (void*)&bat);
    if (retval) {
        perror("task2 thread create");
        exit(1);
    }

    struct timespec ts = {2, 1000*1000};
    nanosleep(&ts, NULL);
    printf("time lapse\n");
    pthread_barrier_wait(&bat);

    printf("enter join\n");
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_attr_destroy(&attr);
    pthread_barrierattr_destroy(&battr);
    pthread_barrier_destroy(&bat);

    return 0;
}
#endif 


