#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	const char* numstr = "0f0fff";
	long int ret = strtol(numstr, NULL, 16);
	printf("ret: %ld\n", ret);
	const char* numstr1 = "0f0fXX";
	ret = strtol(numstr1, NULL, 16);
	printf("ret: %ld\n", ret);
	const char* numstr2 = "QQMMXX";
	ret = strtol(numstr2, NULL, 16);
	printf("ret: %ld\n", ret);
	const char* numstr3 = "0f0Xff";
	ret = strtol(numstr3, NULL, 16);
	printf("ret: %ld\n", ret);
	int num = 4096;
	printf("num: %02d\n", num%99);
	printf("num: %02d\n", num/99);
	char name[12] = {"abcdcdef"};
	char *pos = strchr(name, 'e');
	*pos = '\0';
	printf("name: %s\n", name);
	strcat(name, "efgh");
	printf("name: %s\n", name);

	char  tmp[10] = {0};
	char  dest[10] = {"0123456789"};
	strcpy(dest, tmp);
	printf("dest: %s\n", dest);
	return 0;
}
