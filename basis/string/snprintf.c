
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char result[64] = {0};
	snprintf(result, sizeof(result), "XPON_Common");
	printf("result: %s\n", result);
	return 0;
}
