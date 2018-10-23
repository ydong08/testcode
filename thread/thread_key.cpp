#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* 线程数据 是单个线程私有，其他线程不可见 
*  1. 全局线程变量
*  2. 全局静态线程变量
*  3. 局部静态线程变量 
*/

pthread_key_t key;

void* thread_set(void* p) {
    struct timeval val = {2, 1000};
    int retval = 0;
    for (int i = 0; i < *(int*)p; ++i) {
        retval = pthread_setspecific(key, &i);
        if (retval) {
            perror("set specific");
        }
        select(0, NULL, NULL, NULL, &val);
    }
    pthread_exit(&retval);
}

void* thread_get(void* p) {
    void* pretval = NULL;
    int loop = *(int*)p;
    struct timeval val = {1, 1000};
    do
    {
        pretval = pthread_getspecific(key);
        if (NULL == pretval) {
            printf("no specific\n");
        } else {
            printf("specific: %d\n", *(int*)pretval);
        }
        fflush(stdout);
        select(0, NULL, NULL, NULL, &val);
    } while(0 < --loop);

    pthread_exit(NULL);
}

void clear(void* p) {
    return ;
}

int main() {
    int retval = pthread_key_create(&key, NULL);
    if (retval) {
        printf("errno: %d\n", retval);
        exit(1);
    }

    int setloop = 10;
    int getloop = 20;
    pthread_t setid, getid;

    retval = pthread_create(&setid, NULL, thread_set, &setloop);
    if (retval) {
        printf("errno: %d\n", retval);
        exit(1);
    }
    retval = pthread_create(&getid, NULL, thread_get, &getloop);
    if (retval) {
        printf("errno: %d\n", retval);
        exit(1);
    }

    pthread_join(setid, NULL);
    pthread_join(getid, NULL);
    pthread_key_delete(key);

    return 0;

}

