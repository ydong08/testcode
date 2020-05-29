

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
	const char *act = "action: acconfig";
	const char *cmd = "action: Command";
	const char *table = "Table: ";
	const char *mac = "MAC: ";
	const char *cmdtext = "Command: ";
	const char *delim = "\r\n";
	const char *rule = "Rule: ";

	int  ret  = 0;
	char sendbuf[128] = {0};

	if (argc == 4)
	{
		strcat(sendbuf, cmd);
		strcat(sendbuf, delim);
		strcat(sendbuf, mac);
		strcat(sendbuf, argv[1]);
		strcat(sendbuf, delim);
		strcat(sendbuf, cmdtext);
		strcat(sendbuf, argv[2]);
		strcat(sendbuf, delim);
		strcat(sendbuf, delim);
	}
	else
	{
		strcat(sendbuf, act);
		strcat(sendbuf, delim);
		strcat(sendbuf, mac);
		strcat(sendbuf, argv[1]);
		strcat(sendbuf, delim);
		strcat(sendbuf, table);
		strcat(sendbuf, argv[2]);
		strcat(sendbuf, delim);
		strcat(sendbuf, delim);
	}


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
		printf("send:%d:%s", ret, sendbuf);
		usleep(1000000);
	}


	return 0;
}
