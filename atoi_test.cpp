#include <stdio.h>
#include <stdlib.h>

#if __WORDSIZE == 32
	const int MAXLEN = 10;
#elif __WORDSIZE == 64
	const int MAXLEN = 20;
#endif

#ifdef OS_WIN
	printf("windows system\n");
#endif

#ifdef OS_Mac
	printf("mac system\n");
#endif

#ifdef OS_Linux
	printf("linux system\n");
#endif


int main()
{
	const char* pint = "1234";
	int tmp = atoi(pint);
	printf("%d\n", tmp);

	char buf[MAXLEN] = {0};
	char* p = buf;

	strtol(pint, &p, 10);
	printf("str: %s\n", p);
	
	return 0;
	
}

