

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>   //pthread.h 包含scehd.h
#include <sys/sysinfo.h>

/* 获取cpu的核数 */
int cpu_count() {

    return (int)sysconf(_SC_NPROCESSORS_ONLN); //
    return (int)get_nprocs_conf();  //
    return (int)get_nprocs();   //
}

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
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setaffinity_np(&attr, sizeof(mask), &mask);
    retval = pthread_create(&tid, &attr, thread_func, NULL);
    if (retval < 0) {
        perror("pthread_create");
    }

    retval = pthread_create(&tid, &attr, thread_func1, NULL);
    if (retval < 0) {
        perror("pthread_create");
    }

}



