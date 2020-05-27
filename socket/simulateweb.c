

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
	char *act = "action: acconfig";
	char *table = "Table: ";
	char *mac = "MAC: ";
	char *delim = "\r\n";
	int  ret  = 0;
	char sendbuf[64] = {0};

	strcat(sendbuf, act);
	strcat(sendbuf, delim);
	strcat(sendbuf, mac);
	strcat(sendbuf, argv[1]);
	strcat(sendbuf, delim);
	strcat(sendbuf, table);
	strcat(sendbuf, argv[2]);
	strcat(sendbuf, delim);
	strcat(sendbuf, delim);

	int sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
	{
		perror("socket:");
		return 1;
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = PF_INET;
	sin.sin_port = htons(5435);
	//inet_pton(PF_INET, "192.168.80.101", &sin.sin_addr);
	inet_pton(PF_INET, "127.0.0.1", &sin.sin_addr);

	ret = connect(sockfd, (struct sockaddr*)&sin, (socklen_t)sizeof(sin));
	if (ret < 0)
	{
		perror("connect");
		return 1;
	}

	while(1)
	{
		ret = send(sockfd, sendbuf, strlen(sendbuf), 0);
		if (ret < 0)
		{
			perror("send");
		}
		printf("send\n");
		usleep(1000000);
	}


	return 0;
}
