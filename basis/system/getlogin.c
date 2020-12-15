
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int  retCode = 0;
	char tmpBuf[128] = {0};
	char userid[12] = {0};

	retCode = getlogin_r(tmpBuf, sizeof(tmpBuf));
	if (retCode)
	{
		perror("getlogin");
		return 0;
	}
	printf("name:%s.\n", tmpBuf);
	
	char *ptr = cuserid(userid);

	if (!ptr)
	{
		perror("cuserid");
		return 0;
	}

	printf("USERID:%s.\n", userid);
	return 0;

}
