#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>



int main()
{
	char ipstr[32] = {"134.150.0.163/27"};
	char buf[46] = {0};
	int rc = inet_pton(AF_INET, ipstr, (struct in_addr*)buf);
	if (rc <= 0)
		perror("inet_pton");
	printf("ip hex: %d\n", buf);
	return 0;

}
