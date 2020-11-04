
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void alloc_mem(int fill)
{
	int i;

	for (i = 0; i < 512; ++i) {
		void *p = malloc(1024*1024);
		if (fill) {
			memset(p, 0, 1024*1024);
		}
	}

	printf("Mem is allocated\n");
}

int main(int argc, const char **argv)
{
	alloc_mem(argc >= 2 ? 1 : 0);
	getchar();
	return 0;
}


