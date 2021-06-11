#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* 进程的最初环境变量来自系统变量 
 * 进程内可以通过setenv()设置进程私有的环境变量，
 * 该环境变量可以被其子进程继承 */
extern char **environ;
int main()
{
	int i;
	for (i = 0; environ[i]; ++i)
	{
		printf("environ[%d]: %s.\n", i, environ[i]);
	}

	return 0;
}
