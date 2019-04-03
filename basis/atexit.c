#include <stdio.h>
#include <stdlib.h>

void deal(void)
{
	printf("deal\n");
}

int main()
{
	atexit(deal);
	printf("main\n");
	return 0;

}



