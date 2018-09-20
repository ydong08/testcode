#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_attr_t mattr;
//memset(&mattr, 0x00, sizeof(mattr));
//pthread_mutex_init(&mutex, &mattr);
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
//pthread_cond_attr_t cattr;
//memset(&cattr, 0x00, sizeof(cattr));
//pthread_cond_init(&cond, &cattr);

void handler(void* p) {
    free(p);
    (void)pthread_mutex_destroy((pthread_mutex_t*)p);
}

void* thread1(void* p) {
    pthread_cleanup_push(handler, &mutex);
    while(1) {
        printf("thread1 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("thread1 applied condition\n");
        pthread_mutex_unlock(&mutex);
        sleep(4);
    }
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

void* thread2(void* p) {
    pthread_cleanup_push(handler, &mutex);
    while(1) {
        printf("thread2 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("thread2 applied condition\n");
        pthread_mutex_unlock(&mutex);
        sleep(4);
    }
    pthread_cleanup_pop(0);
    pthread_exit(NULL);
}

int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    sleep(1);
    do {
        pthread_cond_signal(&cond);
    } while(1);
    sleep(20);
    
    return 0;
 
}










