
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void* task1(void* p) {
    pthread_barrier_t pbat = *(pthread_barrier_t*)p;
    printf("task1 will barrier block\n");
    pthread_barrier_wait(&pbat);
    printf("task1 will continue\n");
    pthread_exit(NULL);
}


void* task2(void* p) {
    pthread_barrier_t pbat = *(pthread_barrier_t*)p;
    printf("task2 will barrier block\n");
    pthread_barrier_wait(&pbat);
    printf("task2 will continue\n");
    pthread_exit(NULL);
}

int main() {

    pthread_t tid1, tid2;
    pthread_attr_t attr;
    pthread_barrier_t bat;

    pthread_attr_init(&attr);
    pthread_barrier_init(&bat, 2+1);
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

    pthread_barrier_wait(&bat);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_attr_destroy(&attr);

    return 0;

}