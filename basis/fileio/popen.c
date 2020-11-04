#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


#define CURL_REPORT_COLUMN(x) "\"\"x\"\":\"\"%s\"\","
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
	snprintf(recv, sizeof(recv), CURL_REPORT_COLUMN(ip), "192.168.1.1");
	printf("recv:%s\n", recv);

#if 0
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
#endif
	
	return 0;

}
