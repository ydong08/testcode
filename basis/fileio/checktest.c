#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FILEPATH   "access/test"
int main()
{
	if (!access(FILEPATH, F_OK))
		printf("path exist.\n");
	else
		printf("path nonexist.\n");
		
	return 0;
}
