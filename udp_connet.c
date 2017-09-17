#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define MAXLINE 80
#define SERVER_PORT 8888


int n;
char sendline[MAXLINE];
char recvline[MAXLINE+1];
struct sockaddr_in servaddr;

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("usage:  udpclient serverip\n");
		exit(1);
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(INADDR_ANY);

	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
	{
		printf("[%s] is not a valid IPaddress\n", argv[1]);
		exit(1);
	}

	
	
	#ifdef UDP_CONNECT
		if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
		{
			perror("connect error");
			exit(-1);
		}
	#endif

	while(fgets(sendline, MAXLINE, stdin) != NULL)
	{
		#ifdef UDP_CONNECT
			write(sockfd, sendline, strlen(sendline));
		#else
			sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
		#endif

		printf("write over!\n");

		n = read(sockfd, recvline, MAXLINE);
		if(n == -1)
		{
			perror("read error");
			exit(1);
		}

		recvline[n] = 0;
		fputs(recvline, stdout);

	}

	return 0;

}

http://blog.csdn.net/zhangskd/article/details/13631715

