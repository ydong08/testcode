
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int size = 4 * 1024 * 2;
	char* p = (char*)malloc(size);
	if (p) {
		printf("p addr: %p\n", p);
		free(p);
		memset(p, '1', size);
	}

	if (p)
		printf("p addr2: %p\n", p);
	p = (char*)malloc(size);
	printf("------------");
	if (p)
		printf("p addr3: %p\n", p);
	char* m = (char*)malloc(size*2);
	printf("m addr: %p\n", m);
	return 0;
}
