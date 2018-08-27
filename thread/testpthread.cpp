#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>


#define THREADNUM 5

void* handler2(void* p){
	pid_t pid = getpid();
	pthread_t tpid = pthread_self();
	pthread_detach(tpid);
	printf("     getpid: %d, pthread_self: %ld\n", pid, tpid);

	sleep(2);
	return NULL;
}


void* handler(void* p){
	pid_t pid = getpid();
	pthread_t tpid = pthread_self();
	pthread_detach(tpid);
	printf("   getpid: %d, pthread_self: %ld\n", pid, tpid);
	for (int i = 0; i < THREADNUM-2; ++i){
		pthread_create(&tpid, NULL, handler2, NULL);
	}
	sleep(2);
	return NULL;
}


int main(){
	int sat;
	pthread_t tpid = 0;
	for (int i = 0; i < THREADNUM; ++i){
		pthread_create(&tpid, NULL, handler, NULL);
		printf(" tpid: %ld\n", tpid);
	}
	
	sleep(10);

	return 0;
}