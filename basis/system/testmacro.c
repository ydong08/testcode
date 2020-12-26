
#include <stdio.h>


#define PROCESS1   y
#define PROCESS2   x  

int main()
{
#if PROCESS1
	printf("PROCESS1\n");
#elif PROCESS2 
	printf("PROCESS2\n");
#elif PROCESS3
	printf("PROCESS3\n");
#endif

	return 0;
}
