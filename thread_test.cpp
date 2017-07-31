#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>


void* thread_fun(void* p)
{
	printf("子线程创建OK,  thread ID = %lu\n", (unsigned long)pthread_self());
	pthread_attr_t* attr = (pthread_attr_t*)p;
	int attrdetach;
	if(pthread_attr_getdetachstate(attr, &attrdetach) != 0)
		perror("detached");
	switch(attrdetach ){
		case PTHREAD_CREATE_DETACHED:
			printf("[inner]am detached thread\n");
			break;
		case PTHREAD_CREATE_JOINABLE:
			printf("[inner]am joinable thread\n");
			break;
		default:
			printf("[inner]unknow type thread\n");
			break;
	}
	return NULL;
}


void* entrace(void* p){

	printf("创建子线程成功, thread ID = %lu\n", (unsigned long)pthread_self());
	int inherits;
	pthread_attr_t* attr = (pthread_attr_t*)p;
	//继承性 调度策略 优先级
	pthread_attr_setinheritsched(attr, );
	return NULL;
}


int main()
{
	pthread_attr_t tattr;
	pthread_t pid;
	if (pthread_attr_init(&tattr))
		perror("attr init failed");

	if (pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED) != 0)
		perror("set detach failed");

	pthread_create(&pid, &tattr, thread_fun, (void*)&tattr);


	int status;
	pthread_attr_getdetachstate(&tattr, &status);
	printf("	detachstate: %d\n", status);
	switch(status ){
	case PTHREAD_CREATE_DETACHED:
		printf("[outer]am detached thread\n");
		break;
	case PTHREAD_CREATE_JOINABLE:
		printf("[outer]am joinable thread\n");
		break;
	default:
		printf("[outer]unknow type thread\n");
		break;
	}

	pthread_attr_getschedpolicy(&tattr, &status);
	printf("	schedpolicy: %d\n", status);

	pthread_attr_getinheritsched(&tattr, &status);
	printf("	inheritsched: %d\n", status);

	pthread_attr_getscope(&tattr, &status);
	printf("	scope: %d\n", status);

	size_t stat;
	pthread_attr_getguardsize(&tattr, &stat);
	printf("	guardsize: %zu\n", stat);

	//pthread_attr_getstackaddr(&tattr, &status);
	//printf("	stackaddr: %p", status);

	pthread_attr_getstacksize(&tattr, &stat);
	printf("	stacksize: %zu\n", stat);


	pthread_attr_destroy(&tattr);
	sleep(3);
	printf("int max: %u\n", (1<<-1) - 1);
	printf("int max: %u\n", ((unsigned int)-1) >>1);
	
	return 0;

}

