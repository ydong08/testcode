
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main()
{
	printf("getuid():%d, geteuid():%d\n", getuid(), geteuid());
	return 0;
}
