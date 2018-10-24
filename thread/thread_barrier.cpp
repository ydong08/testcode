
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#if 0
void* task1(void* p) {
    pthread_barrier_t bat = *(pthread_barrier_t*)p;
    printf("task1 will barrier block\n");
    pthread_barrier_wait(&bat);
    printf("task1 will continue\n");
    pthread_exit(NULL);
}


void* task2(void* p) {
    pthread_barrier_t bat = *(pthread_barrier_t*)p;
    printf("task2 will barrier block\n");
    pthread_barrier_wait(&bat);
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

    return 0;

}
#endif 

#if 1

#include <unistd.h>

void *Task1(void *arg);
void *Task2(void *arg);

int main(void)
{
    pthread_t tid, tid2;
    pthread_attr_t attr;
    pthread_barrier_t barrier;
    //初始化线程属性
    pthread_attr_init(&attr);
    pthread_barrier_init(&barrier,NULL,2 + 1);//2+1个等待

    //创建线程1
    pthread_create(&tid, &attr,Task1,&barrier);

    //创建线程2
    pthread_create(&tid2, &attr,Task2,&barrier);
    printf("main process will sleep 6s.\r\n");
    sleep(6);/*等待6s后,才让线程运行*/
    pthread_barrier_wait(&barrier);//起跑枪“砰!”

    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);
    pthread_barrier_destroy(&barrier);

    return 0;
}

void *Task1(void *arg)
{
    pthread_barrier_t barrier = *(pthread_barrier_t*)arg;
    printf("Task1 will be blocked.\r\n");
    pthread_barrier_wait(&barrier);//所有线程都被阻塞在这里
    printf("Task1 is running.\r\n");
    sleep(3);//延时3s
    pthread_exit(NULL);
}

void *Task2(void *arg)
{
    pthread_barrier_t barrier = *(pthread_barrier_t*)arg;
    printf("Task2 will be blocked.\r\n");
    pthread_barrier_wait(&barrier);//所有线程都被阻塞在这里
    printf("Task2 is running.\r\n");
    sleep(3);//延时3s
    pthread_exit(NULL);
}

#endif