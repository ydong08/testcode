#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include <sys/syscall.h>

/* KP1 ~ KP4 */

#define THREADNUM 5
// KP4: 在线程中创建线程,并区别线程ID
void* handler2(void* p){

	pid_t pid = getpid();
	pthread_t tpid = pthread_self();
	pthread_detach(tpid);
	printf("     getpid: %d, pthread_self: %ld\n", pid, tpid);
	// KP3: tid,内核线程ID,其实是普通进程
	pid = syscall(SYS_gettid);
	printf("     gettid: %d\n", pid);
	sleep(2);
	pthread_exit(NULL);
}


void* handler(void* p){
	
	// KP1: pid  进程ID
	pid_t pid = getpid();
	
	// KP2: thread id,posix 定义的线程ID,可复用,
	// 实际是线程控制块tcb首地址相对于进程数据的段的偏移,故不同进程中的线程可能有相同的id
	pthread_t tpid = pthread_self();
	pthread_detach(tpid);
	printf("   getpid: %d, pthread_self: %ld\n", pid, tpid);

	// KP3: tid,内核线程ID,其实是普通进程
	pid = syscall(SYS_gettid);
	printf("   gettid: %d\n", pid);
	for (int i = 0; i < THREADNUM-2; ++i){
		pthread_create(&tpid, NULL, handler2, NULL);
	}
	sleep(2);

	pthread_exit(NULL);
}


int main(){
	pthread_t tpid = 0;
	for (int i = 0; i < THREADNUM; ++i){
		pthread_create(&tpid, NULL, handler, NULL);
		printf(" tpid: %ld\n", tpid);
	}
	
	sleep(10);

	return 0;
}