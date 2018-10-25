

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>

int data;
pthread_mutex_t mtx;
void* proc(void* p) {
    struct timespec ts = {6, 1000000};
    printf("proc blocked\n");
    pthread_mutex_lock(&mtx);
    nanosleep(&ts, NULL);
    for (int i = -10; i < 0; ++i) {
        data = i;
        printf("%d,", data);
    }
    printf("\n");
    pthread_mutex_unlock(&mtx);
    printf("proc exit blocked\n");
}

void prepare() {
    printf("lock mutex before fork\n");
    pthread_mutex_lock(&mtx);
}

void parent() {
    printf("unlock mutex in parent before fork return\n");
    pthread_mutex_unlock(&mtx);
}

void child() {
    printf("unlock mutex in child before fork return\n");
    pthread_mutex_unlock(&mtx);
}


int main() {
    pthread_t tid;
    pthread_mutex_init(&mtx, NULL);
    int retval = pthread_create(&tid, NULL, proc, NULL);
    if (retval) {
        perror("thread create");
        exit(1);
    }

    int pid = pthread_atfork(prepare, parent, child);
    if (pid < 0) {
        perror("atfork fail");
        exit(1);
    } else if (0 == pid) { // child
        pthread_mutex_lock(&mtx);
        for (int i = 0; i < 10; ++i) {
            data = i;
            printf("%d,", data);
        }
        printf("\n");
        pthread_mutex_unlock(&mtx);
        exit(0);
    } else {
        pthread_mutex_lock(&mtx);
        for (int i = 10; i < 20; ++i) {
            data = i;
            printf("%d,", data);
        }
        printf("\n");
        pthread_mutex_unlock(&mtx);
        waitpid(pid, NULL, NULL);
    } 

    pthread_mutex_destroy(&mtx);
    return 0;

}