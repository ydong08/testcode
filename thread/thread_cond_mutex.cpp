#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>

int sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_attr_t mattr;
//memset(&mattr, 0x00, sizeof(mattr));
//pthread_mutex_init(&mutex, &mattr);
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
//pthread_cond_attr_t cattr;
//memset(&cattr, 0x00, sizeof(cattr));
//pthread_cond_init(&cond, &cattr);
bool sigFlag = false;
struct timeval tmv = {2, 1000};
void sigHandler(int signo, siginfo_t *p, void* po) {
    if (SIGALRM == signo) {
        sigFlag = true;
        printf("recv SIGALRM signal\n");
    }
}
void handler(void* p) {
    printf("enter cancel handler: thread[%d]\n", *(int*)p);
    (void)pthread_mutex_destroy(&mutex);
    (void)pthread_cond_destroy(&cond);
}

void ignoreSig(int signo) {
    sigset_t sigt;
    sigset_t sigo;
    sigemptyset(&sigt);
    sigemptyset(&sigo);
    sigaddset(&sigt, signo);
    sigprocmask(SIG_BLOCK, &sigt, &sigo);
}

void* thread1(void* p) {
    //ignoreSig(SIGALRM);
    //long tids = syscall(SYS_gettid);
    int tid = 1;
    pthread_cleanup_push(handler, &tid);
    while(1) {
        //printf("thread1： begin lock\n");
        pthread_mutex_lock(&mutex);
        //printf("thread1： begin wait\n");
        pthread_cond_wait(&cond, &mutex);
        //printf("thread1： begin unlock\n");
        pthread_mutex_unlock(&mutex);
        //printf("thread1: end unlock\n");
        select(0, NULL, NULL, NULL, &tmv);
    }
    //printf("thread1: begin pop\n");
    pthread_cleanup_pop(0);
    //printf("thread1: begin exit\n");
    pthread_exit((void*)1);
}

void* thread2(void* p) {
    //ignoreSig(SIGALRM);
    //long tids = syscall(SYS_gettid);
    int tid = 2;
    pthread_cleanup_push(handler, &tid);
    while(0 < *(int*)p) {
        //printf("thread2： begin lock\n");
        pthread_mutex_lock(&mutex);
        //printf("thread2： begin wait\n");
        pthread_cond_wait(&cond, &mutex);
        printf("getsum: %d\n", sum);
        sum = 0;
        //printf("thread2： begin unlock\n");
        pthread_mutex_unlock(&mutex);
        //printf("thread2: end unlock\n");
        //select(0, NULL, NULL, NULL, &tmv);
    }
    //printf("thread2: begin pop\n");
    pthread_cleanup_pop(0);
    //printf("thread2: begin exit\n");
    pthread_exit((void*)2);
}

void setTimer(long int interSec, long int valueSec) {
    // set timeout
    struct itimerval itv;
    struct timeval tp;
    tp = {interSec, 10};
    itv.it_interval = tp;
    itv.it_value = {valueSec, 10};
    setitimer(ITIMER_REAL, &itv, NULL);
    // register sig callback
    struct sigaction act;
    struct sigaction acto;
    memset(&act, 0x00, sizeof(act));
    memset(&acto, 0x00, sizeof(acto));
    act.sa_flags = SA_SIGINFO | SA_RESTART;
    act.sa_sigaction = sigHandler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGALRM);
    sigaction(SIGALRM, &act, &acto);
}

bool isReachTime(void) {
    return sigFlag;
}


int main() {
    int add = 10;
    int loop = 10;
    pthread_t tid1;
    volatile int param = 1;
    pthread_create(&tid1, NULL, thread2, (void*)&param);
    //pthread_create(&tid2, NULL, thread2, NULL);
    //setTimer(2, 10);
    do {
        // if (isReachTime())
        //     // 如果不调用signal或者broadcast,线程将保持睡眠状态，不会被唤醒执行
        pthread_mutex_lock(&mutex);
        for(int i = 0; i < add; ++i) {
            for (int j = 0; j < add; ++j) {
                sum += j;
            }
        }
        printf("setsum: %02d\n", sum);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        if (loop == 1) {
            //pthread_cancel(tid1);
            usleep(1000);
            param = 0;
            //pthread_cancel(tid2);
        }

        usleep(1000000);
    } while(0 < loop--);
    
    void* st ;
    pthread_join(tid1, (void**)&st);
    printf("tid1 st: %ld\n", st);

    // pthread_join(tid2, (void**)&st);
    // printf("tid2 st: %ld\n", st);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}


