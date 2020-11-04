
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char result[64] = {0};
	snprintf(result, sizeof(result), "XPON_Common");
	printf("result: %s\n", result);

	snprintf(result, sizeof(result), "\"XPON_Common\"");
	printf("result: %s\n", result);

	printf("len:%d\n", sizeof("\"code\"=\""));
	printf("len:%d\n", strlen("\"code\"=\""));

	const char* pnum = " 35678";
	float num = (float)(atoi(pnum))/1024;
	printf("num:%.2f.\n", num);

	static int speed[32] = {0};
	printf("sizeof(speed):%d.\n", sizeof(speed));
	
	return 0;
}
