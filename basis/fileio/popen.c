#include <stdio.h>
#include <stdlib.h>


int main()
{
	int i = 0;
	const char *curl = "/usr/bin/curl www.baidu.com";
	const char *f = "/home/winter/repo/testcode/basis/fileio/sleep.sh";
	const char *cmd = "echo 123; sleep 10; echo 456;";
	char  recv[128] = {0};

	//FILE *fp = popen(cmd, "r");
	FILE *fp = popen(f, "r");
	if (fp) pclose(fp);


	for (i = 0; i < 100; ++i)
	{
		memset(recv, 0, sizeof(recv));
		fp = popen(curl, "r");
		if (fp)
		{
			fread(recv, sizeof(recv) - 1, 1, fp);
			printf("recv:%s\n", recv);
			pclose(fp);
			usleep(500000);
		}
	}

	return 0;

}
