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

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_attr_t mattr;
//memset(&mattr, 0x00, sizeof(mattr));
//pthread_mutex_init(&mutex, &mattr);
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
//pthread_cond_attr_t cattr;
//memset(&cattr, 0x00, sizeof(cattr));
//pthread_cond_init(&cond, &cattr);
bool sigFlag = false;
void sigHandler(int signo, siginfo_t *p, void* po) {
    if (SIGALRM == signo) {
        sigFlag = true;
        printf("recv SIGALRM signal\n");
    }
}
void handler(void* p) {
    printf("enter cancel handler: thread[%d]\n", *(int*)p);
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
    ignoreSig(SIGALRM);
    //long tids = syscall(SYS_gettid);
    int tid = 1;
    pthread_cleanup_push(handler, &tid);
    while(1) {
        printf("thread1 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("thread1 applied condition\n");
        pthread_mutex_unlock(&mutex);
        sleep(6);
    }
    pthread_cleanup_pop(0);
    pthread_exit((void*)1);
}

void* thread2(void* p) {
    ignoreSig(SIGALRM);
    //long tids = syscall(SYS_gettid);
    int tid = 2;
    pthread_cleanup_push(handler, &tid);
    while(1) {
        printf("thread2 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("thread2 applied condition\n");
        pthread_mutex_unlock(&mutex);
        sleep(6);
    }
    pthread_cleanup_pop(0);
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
    int loop = 10;
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    //setTimer(2, 10);
    do {
        if (isReachTime())
            // 如果不调用signal或者broadcast,线程将保持睡眠状态，不会被唤醒执行
            pthread_cond_signal(&cond);
        if (loop == 1) {
            pthread_cancel(tid1);
            usleep(1000);
            pthread_cancel(tid2);
        }

        usleep(1000000);
    } while(0 < loop--);
    
    void* st ;
    pthread_join(tid1, (void**)&st);
    printf("tid1 st: %d\n", st);

    pthread_join(tid2, (void**)&st);
    printf("tid2 st: %d\n", st);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}










