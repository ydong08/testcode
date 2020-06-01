
#include <stdio.h>
#include <stdlib.h>

/* error syntax */
int main() {
	int loop = 1;
	if (loop)
		printf("loop\n");
	else {
		if (loop)
			printf("else1");
	} else
		printf("else2\n");


	return 0;
}
