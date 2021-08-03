#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

/* 本函数测试popen函数执行脚本时是否需要脚本必须具有可执行权限 */
#define  TEST_SCRIPT     "./test_script.sh"
int main()
{
	FILE *fp = NULL;
	char buf[32] = {0};
	chmod(TEST_SCRIPT, S_IXUSR|S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	fp = popen(TEST_SCRIPT, "r");
	if (!fp)
	{
		perror("popen:");
	}

	fread(buf, 1, sizeof(buf), fp);
	printf("content: %s.\n", buf);
	pclose(fp);
	
	memset(buf, 0, sizeof(buf));
	int num = atoi(buf);
	printf("num:%d.\n", num);

	return 0;
}

/* 结论: popen函数执行的脚本 必须 有可执行权限 */
