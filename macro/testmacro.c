
#include <stdio.h>


#define PROCESS1   y
#define PROCESS2   x  

int main()
{
#if (PROCESS1 == y)
	printf("PROCESS1\n");
#elif (PROCESS2 == x)
	printf("PROCESS2\n");
#elif (PROCESS3 == z)
	printf("PROCESS3\n");
#endif

	return 0;
}
