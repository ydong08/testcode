

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>


/* KP1 ~ KP7 */
int data;
pthread_mutex_t mtx;
void* proc(void* p) {
    struct timespec ts = {1, 1000000};
    printf("proc blocked\n");
    pthread_mutex_lock(&mtx);
    // KP1:  多线程中适用nanosleep和select函数作延时
    nanosleep(&ts, NULL);
    for (int i = -10; i < 0; ++i) {
        data = i;
        printf("%d,", data);
    }
    printf("\n");
    pthread_mutex_unlock(&mtx);
    printf("proc exit blocked\n");
    pthread_exit(NULL);
}

// KP2: 创建子进程前将所有锁加锁锁住
void prepare() {
    printf("lock mutex before fork\n");
    pthread_mutex_lock(&mtx);
}

// KP3: 在fork返回前释放父进程中的所有锁
void parent() {
    printf("unlock mutex in parent before fork return\n");
    pthread_mutex_unlock(&mtx);
}

// KP4: 在fork返回前释放子进程中的所有锁
void child() {
    printf("unlock mutex in child before fork return\n");
    pthread_mutex_unlock(&mtx);
}


int main() {
    pthread_t tid;
    pthread_mutex_init(&mtx, NULL);
    /* 疑问: tid线程是在fork前创建,按照fork的语义,fork后的子进程是父进程的副本,
            子进程中是否应该也有一份tid线程?
            从代码的执行结果看,tid线程只执行了一次.
    */
    int retval = pthread_create(&tid, NULL, proc, NULL);
    if (retval) {
        perror("thread create");
        exit(1);
    }

    // KP5: fork前清空缓冲区
    fflush(stdout);
    // KP6: 注册锁处理函数
    pthread_atfork(prepare, parent, child);
    // KP7: 创建子进程
    int pid = fork();
    if (pid < 0) {
        perror("atfork fail");
        pthread_join(tid, NULL);
        pthread_mutex_destroy(&mtx);
        exit(1);
    } else if (0 == pid) { // child
        pthread_mutex_lock(&mtx);
        for (int i = 0; i < 10; ++i) {
            data = i;
            printf("%d,", data);
        }
        printf("\n");
        pthread_mutex_unlock(&mtx);
        //exit(0);
    } else { // parent
        pthread_mutex_lock(&mtx);
        for (int i = 10; i < 20; ++i) {
            data = i;
            printf("%d,", data);
        }
        printf("\n");
        pthread_mutex_unlock(&mtx);
        //waitpid(pid, NULL, NULL);
        wait(NULL);
    } 

    struct timespec ts = {2, 1000000};
    nanosleep(&ts, NULL);
    pthread_join(tid, NULL);
    pthread_mutex_destroy(&mtx);
    return 0;

}

