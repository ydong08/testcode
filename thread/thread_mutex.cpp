#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;
int tmp = 10;
//pthread_mutex_init(&mutex, NULL);
void* thread(void* p) {
    pthread_mutex_lock(&mutex);
    tmp += 10;
    /**
     *  pthread_self()   是POSIX标准描述的线程ID，实际是线程控制块TCB的首地址，用于区分同一进程的不同线程
     *  gettid()     是内核中真实的线程ID
     * */
    printf("tid[%ld] thread[%lu] asign new value: %d\n", syscall(SYS_gettid), pthread_self(), tmp);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main() {
    pthread_t tid, tid2, tid3;
    int retval = pthread_create(&tid, NULL, thread, NULL);
    if (retval)
        perror("pthread_create");

    retval = pthread_create(&tid2, NULL, thread, NULL);
    if (retval)
        perror("pthread_create");

    retval = pthread_create(&tid3, NULL, thread, NULL);
    if (retval)
        perror("pthread_create");

    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_mutex_destroy(&mutex);

    return 0;

}




