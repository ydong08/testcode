


/**
* flockfile function wait until the FILE object is no longer locked by a different thread, 
* then lock it, do the requested I/O, and unlock the object again
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

/* callback */
void* writefile(void* p)
{
	int loop = 50;
	char buf[4] = {"abcd"};
	if (!p)
		return NULL;
	
	FILE* fp = (FILE*)p;
	flockfile(fp);
	printf("flockfile enter\n");
	do
	{
		usleep(1000000);
		fwrite(buf, sizeof(buf), 1, fp);
	} while(0 < --loop);
	funlockfile(fp);
	printf("flockfile out\n");
}

#define NUM   6
int main()
{
	int ret = -1;
	pthread_t tid[NUM] = {0};
	/* open file */
	const char* file="./filelock.log";
	FILE* fp = fopen(file, "a+");
	if (!fp) 
	{
		printf("fopen error:%s\n",strerror(errno));
		return -1;
	}
	printf("openfile ok\n");
	/* create thread */
	for (int i = 0; i < NUM; ++i)
	{
		ret = pthread_create(&tid[i], NULL, writefile, fp);
		if (ret)
			printf("create thread[%d] failed\n", i);
		else
			printf("create thread[%d:%ld] ok\n", i, tid[i]);
	}

	for (int i = 0; i < NUM; ++i)
	{
		pthread_join(tid[i], NULL);
		printf("capture thread[%d]\n", tid[i]);
	}

	return 0;
}


