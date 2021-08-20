#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define SERV_PORT 8080
#define LIST 20
#define MAX_FD 10


int main(int argc, char *argv[])
{
	int sockfd;
	int err;
	int i;
	int connfd;
	int fd_all[MAX_FD];
	
	fd_set fd_read;
    fd_set fd_select; 

	struct timeval timeout;
	struct timeval timeout_select;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	socklen_t serv_len;
	socklen_t cli_len;
	
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("fail to socket");
		exit(1);
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // ip

    serv_len = sizeof(serv_addr);
	
	err = bind(sockfd, (struct sockaddr *)&serv_addr, serv_len);
	if(err < 0)
	{
		perror("fail to bind");
		exit(1);
	}

	err = listen(sockfd, LIST);
	if(err < 0)
	{
		perror("fail to listen");
		exit(1);
	}
	
	memset(fd_all, -1, sizeof(fd_all));

	fd_all[0] = sockfd;
	
	FD_ZERO(&fd_read);
	FD_SET(sockfd, &fd_read);

	int maxfd = fd_all[0]; 
	
	while(1)
	{
        fd_select = fd_read;
        timeout_select = timeout;
		
		err = select(maxfd+1, &fd_select, NULL, NULL, NULL);
		if(err < 0)
		{
				perror("fail to select");
				exit(1);
		}

		if(err == 0){
            printf("timeout\n");
		}
		
		if( FD_ISSET(sockfd, &fd_select) )
		{
            cli_len = sizeof(cli_addr);
			connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
			if(connfd < 0)
			{
				perror("fail to accept");
				exit(1);
			}
			
			char cli_ip[INET_ADDRSTRLEN] = {0};
			inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
			printf("----------------------------------------------\n");
			printf("client ip=%s,port=%d\n", cli_ip,ntohs(cli_addr.sin_port));
			
			for(i=0; i < MAX_FD; i++){
				if(fd_all[i] != -1){
					continue;
				}else{
					fd_all[i] = connfd;
					printf("client fd_all[%d] join\n", i);
					break;
				}
			}
			
			FD_SET(connfd, &fd_read);
			if(maxfd < connfd)
			{
				maxfd = connfd;
			}
		}
		
		for(i=1; i < maxfd; i++)
		{
			if(FD_ISSET(fd_all[i], &fd_select))
			{
				printf("fd_all[%d] is ok\n", i);
				char buf[1024]={0};
				int num = read(fd_all[i], buf, 1024);
				if(num > 0){
					printf("receive buf from client fd_all[%d] is: %s\n", i, buf);
					num = write(fd_all[i], buf, num);
					if(num < 0){
						perror("fail to write ");
						exit(1);
					}else{
						//printf("send reply\n");
					}
				}
				else if(0 == num){
					printf("client:fd_all[%d] exit\n", i);
					FD_CLR(fd_all[i], &fd_read);
					close(fd_all[i]);
					fd_all[i] = -1;
					
					continue;
				}
				
			}else {
                //printf("no data\n");                  
            }
		}
	}
	
    return 0;
}
