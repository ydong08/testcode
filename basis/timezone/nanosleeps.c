
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>



int main()
{
	int count = 10;
	struct timespec mts;
	struct timespec nts;

	mts.tv_sec = 1;
	mts.tv_nsec = 1000;

	do
	{
		nanosleep(&mts, &nts);
		printf("nanosleep\n");

	} while(0 < count--);

}
