#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

/* KP1 ~ KP5 */
// KP1: 线程本地存储TLS(ThreadLocalStorage)
/* 线程数据 是单个线程私有，其他线程不可见 
*  1. 全局线程变量
*  2. 全局静态线程变量
*  3. 局部静态线程变量 
*/


// 1. 全局线程变量
int gNum = 100;

// 2. 全局静态线程变量
static int sAge = 28;

typedef struct thread_info {
    pthread_t tid;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
} thread_info_t;

void clear(void* p) {
    return ;
}

void* thread_set(void* p) {
    pthread_mutex_t mtx = ((thread_info_t*)p)->mtx;
    pthread_cond_t cond = ((thread_info_t*)p)->cond;
    pthread_t tid = ((thread_info_t*)p)->tid;
    struct timeval val;
    int retval;
    // pthread_cleanup_push(clear, NULL);
    do {
        val.tv_sec = 2;
        val.tv_usec = 1000;
        // pthread_mutex_lock(&mtx);
        printf("set num = %d\n", sAge);
        // sAge = loop;
        // pthread_cond_signal(&cond);
        // KP2: 线程取消(取消点可查看 man 7 threads)
        if(120 < sAge) 
            pthread_cancel(tid);
        
        // KP3: 测试线程状态
        retval = pthread_kill(tid, 0);
        if(ESRCH == retval) {
            printf("thread exit\n");
            break;
        }
        // pthread_mutex_unlock(&mtx);
        // val 的值会被更新为剩余的时间,故每次循环需重新设定时间
        // KP4: 多线程下select高精度延时的用法
        select(0, NULL, NULL, NULL, &val); 
    } while(1);
    // pthread_cleanup_pop(0);
    
    pthread_exit(NULL);
}

void* thread_get(void* p) {
    pthread_key_t key;
    void* pretval = NULL;
    int retval;
    int oldtype;
    struct timeval val;
    pthread_mutex_t mtx = ((thread_info_t*)p)->mtx;
    pthread_cond_t cond = ((thread_info_t*)p)->cond;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldtype);
    printf("cancelstate: %d\n", oldtype);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldtype);
    printf("canceltype: %d\n", oldtype);
    pthread_cleanup_push(clear, NULL);
    do
    {
        val.tv_sec = 1;
        val.tv_usec = 1000;
        retval = pthread_mutex_lock(&mtx);
        if (retval) {
            perror("lock");
            break;
        }
        // retval = pthread_cond_wait(&cond, &mtx);
        // if (retval) {
        //     perror("cond wait");
        //     break;
        // }
        // KP5: TLS的API用法
        retval = pthread_key_create(&key, NULL);
        if (retval) {
            perror("key create");
            break;
        }

        retval = pthread_setspecific(key, &sAge);
        if (retval) {
            perror("set specific");
            break;
        }
        pretval = pthread_getspecific(key);
        if (NULL == pretval) {
            printf("no specific\n");
        } else {
            printf("get num = %d\n", *(int*)pretval);
        }
        fflush(stdout);
        sAge++;
        pthread_mutex_unlock(&mtx);
        select(0, NULL, NULL, NULL, &val);
    } while(1);
    pthread_cleanup_pop(0);
    pthread_exit(&retval);
}


int main() {

    pthread_t setid, getid;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cond, NULL);

    thread_info_t stinfo;
    stinfo.mtx = mtx;
    stinfo.cond = cond;
    int retval = pthread_create(&getid, NULL, thread_get, (void*)&stinfo);
    if (retval) {
        printf("errno: %d\n", retval);
        exit(1);
    }
    stinfo.tid = getid;
    retval = pthread_create(&setid, NULL, thread_set, (void*)&stinfo);
    if (retval) {
        printf("errno: %d\n", retval);
        exit(1);
    }

    pthread_join(getid, NULL);
    pthread_join(setid, NULL);
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);

    return 0;

}

