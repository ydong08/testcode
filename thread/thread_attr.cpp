#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>


void* thread_fun(void* p)
{
	printf("enter thread fun\n");

	return NULL;
}


int main()
{
	pthread_attr_t tattr;
	pthread_t pid;
	if (pthread_attr_init(&tattr))
		perror("attr init failed");
	int status;
	pthread_attr_getdetachstate(&tattr, &status);
	printf("	detachstate: %d\n", status);

	pthread_attr_getschedpolicy(&tattr, &status);
	printf("	schedpolicy: %d\n", status);

	pthread_attr_getinheritsched(&tattr, &status);
	printf("	inheritsched: %d\n", status);

	pthread_attr_getscope(&tattr, &status);
	printf("	scope: %d\n", status);

	size_t stat;
	pthread_attr_getguardsize(&tattr, &stat);
	printf("	guardsize: %d\n", stat);

	//pthread_attr_getstackaddr(&tattr, &status);
	//printf("	stackaddr: %p", status);

	pthread_attr_getstacksize(&tattr, &stat);
	printf("	stacksize: %d\n", stat);

	if (pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED) != 0)
		perror("set detach failed");

	pthread_create(&pid, &tattr, thread_fun, NULL);

	pthread_attr_destroy(&tattr);
	return 0;

}

