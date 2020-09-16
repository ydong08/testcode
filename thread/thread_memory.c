

#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <sys/syscall.h>  
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>  
#include <string.h>
#include <time.h>
  
#define gettid() syscall(__NR_gettid)  /*get LWP ID */  
  
pthread_key_t key;  
  
__thread int count = 42;  
__thread unsigned long long count2 ;  
static __thread int count3;  
  
/*thread key destructor*/  
void keydestr(void* string)  
{  
    printf("destructor excuted in thread %p,address (%p) param=%s\n",pthread_self(),string,string);  
    free(string);  
}  
  
void * thread1(void *arg)  
{  
    int b; 
    int loop = 60; 
    struct timespec mts;
    pthread_t tid=pthread_self();  
    size_t size = 8;  
  
    int autovar = 0;  
    static int staticvar = 1;  
    printf("In thread1, autovaraddress = %p, staticvaraddress = %p\n", &autovar, &staticvar);  
  
    printf("In thread1, tid = %p, gettid = %d\n",tid,gettid());  
  
    char* key_content = ( char* )malloc(size);  
    if(key_content != NULL)  
    {  
        strcpy(key_content,"maximus0");  
    }  
    pthread_setspecific(key,(void *)key_content);  

    count = 1024;  
    count2 = 2048;  
    count3 = 4096;  
    printf("In thread1, tid=%p, count(%p) = %8d, count2(%p) = %6llu, count3(%p) = %6d\n",tid,&count,count,&count2,count2,&count3,count3);  
  
  	mts.tv_sec = 1;
  	mts.tv_nsec = 200000000;
  	do
    {
    	printf("In thread1, tid=%p, count(%p) = %8d, count2(%p) = %6llu, count3(%p) = %6d\n",tid,&count,count,&count2,count2,&count3,count3);  
    	nanosleep(&mts, NULL);
    } while(0 < loop--);

    sleep(2);  
    printf("thread1 %p keyselfaddress = %p, returns keyaddress = %p\n",tid,&key, pthread_getspecific(key));  
  
    sleep(30);  
    printf("thread1 exit\n");  
}  
  
void * thread2(void *arg)  
{  
    int b; 
    int loop = 60;  
    struct timespec mts;
    pthread_t tid=pthread_self();  
    size_t size = 8;  
  
    int autovar = 0;  
    static int staticvar = 1;  
    printf("In thread2, autovaraddress = %p, staticvaraddress = %p\n", &autovar, &staticvar);  
  
    printf("In thread2, tid = %p, gettid = %d\n",tid,gettid());  
  
    char* key_content = ( char* )malloc(size);  
    if(key_content != NULL)  
    {  
        strcpy(key_content,"ABCDEFG");  
    }  
    pthread_setspecific(key,(void *)key_content);  
  
    count = 1025;  
    count2 = 2049;  
    count3 = 4097;  
    printf("In thread2, tid=%p, count(%p) = %8d, count2(%p) = %6llu, count3(%p) = %6d\n",tid,&count,count,&count2,count2,&count3,count3); 

    mts.tv_sec = 1;
  	mts.tv_nsec = 200000000;
  	do
    {
    	printf("In thread2, tid=%p, count(%p) = %8d, count2(%p) = %6llu, count3(%p) = %6d\n",tid,&count,count,&count2,count2,&count3,count3);  
    	nanosleep(&mts, NULL);
    } while(0 < loop--);
  
    sleep(1);  
    printf("thread2 %p keyselfaddress = %p, returns keyaddress = %p\n",tid,&key, pthread_getspecific(key));  
  
    sleep(50);  
    printf("thread2 exit\n");  
}  
  
  
int main(void)  
{  
    int b;  
    int autovar = 0;  
    static int staticvar = 1; 
    int loop = 80; 
    struct timespec mts; 
    int ict = 1024;
    unsigned long long ict1 = 2048;
    int ict2 = 4096;
  
    pthread_t tid1,tid2;  
    printf("start,pid=%d\n",getpid());  
  
    printf("In main, autovaraddress = %p, staticvaraddress = %p\n", &autovar, &staticvar);  
  
    pthread_key_create(&key,keydestr);  
  
    pthread_create(&tid1,NULL,thread1,NULL);  
    pthread_create(&tid2,NULL,thread2,NULL);  
  
    printf("In main, pthread_create tid1 = %p\n",tid1);  
    printf("In main, pthread_create tid2 = %p\n",tid2);  
  
  	mts.tv_sec = 0;
  	mts.tv_nsec = 800000000;
  	do
    {
    	count = ict++;
    	count2 = ict1++;  
    	count3 = ict2++; 
    	printf("In main, tid=%d, count(%p) = %8d, count2(%p) = %6llu, count3(%p) = %6d\n",getpid(),&count,count,&count2,count2,&count3,count3);  
    	nanosleep(&mts, NULL);
    } while(0 < loop--);

    if(pthread_join(tid2,NULL) == 0)  
    {  
        printf("In main,pthread_join thread2 success!\n");  
        sleep(5);  
    }  
  
    pthread_key_delete(key);  
    printf("main thread exit\n");  
  
    return 0;  
}  
