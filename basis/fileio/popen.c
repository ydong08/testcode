#include <stdio.h>
#include <stdlib.h>


int main()
{
	const char *f = "/home/winter/repo/testcode/basis/fileio/sleep.sh";
	const char *cmd = "echo 123; sleep 10; echo 456;";

	//FILE *fp = popen(cmd, "r");
	FILE *fp = popen(f, "r");
	if (fp) pclose(fp);

	return 0;

}
