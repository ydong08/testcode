

#include <stdio.h>
#include <stdlib.h>
//#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>   //pthread.h 包含scehd.h

void wast_time() {
    int b = 10000000;
    int tp = 0;
    while(0 < b--) {
        tp = 10000*10000;
    }
    struct timeval val = {10, 1000};
    select(0, NULL, NULL, NULL, &val);
}

void* thread_func(void* p) {
    pthread_detach(pthread_self());
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    int retval = pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);
    if (retval < 0) {
        perror("set cpu 0");
    }

    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
    retval = pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);
    if (retval < 0) {
        perror("set cpu 1");
    }
    pthread_exit(NULL);

}

void* thread_func1(void* p) {
    pthread_detach(pthread_self());
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(2, &mask);
    int retval = pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);
    if (retval < 0) {
        perror("set cpu 2");
    }

    CPU_ZERO(&mask);
    CPU_SET(3, &mask);
    retval = pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);
    if (retval < 0) {
        perror("set cpu 3");
    }
    pthread_exit(NULL);
}

int main() {

    sleep(60);
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(1, &mask);
    int retval = sched_setaffinity(0, sizeof(mask), &mask);
    if (retval < 0) {
        perror("set cpu 1");
    }

    pthread_t tid;
    retval = pthread_create(&tid, NULL, thread_func, NULL);
    if (retval < 0) {
        perror("pthread_create");
    }

    retval = pthread_create(&tid, NULL, thread_func1, NULL);
    if (retval < 0) {
        perror("pthread_create");
    }

}

