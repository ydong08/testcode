/*
 * http_server.c
 *
 *  Created on: 2015-1-7
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <errno.h>

typedef struct _fd_info
{
	fd_set fds;
	int newfd;
} fd_info;

int read_line(int fd,char *buf,int *size)
{
	int  len = 0;
	int  recvlen = 0;

	do
	{
		len = recv(fd, buf + recvlen, *size - recvlen, 0);
		//printf("recv done:%d-errno:%d\n", len, errno);
		if (len < 0)
		{
			//perror("recv");
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				break;
			}

			recvlen = -1;
			break;
		}

		if (len <= 0 || *size <= recvlen)
		{
			*size = recvlen;
			break;
		}

		recvlen += len; 

	} while(1);

	return recvlen;
}

void do_write(int fd, int flag)
{
	int  len = 0;
	char *header = "HTTP/1.1 200 OK\r\nContent-Type: application/json;charset=utf-8\r\nContent-Length: %d\r\n";
	char *body = "<html><body>%s</body></html>\r\n";
	char *response0 = "{\"speed\":\"800M\",\"threadcount\":\"4\",\"ip\":\"192.168.28.92\",\"port\":\"\",\"download\":\"3.7G.iso\",\"identify\":\"8e004f46-1dbb-44ea-a53b-126587ab9675\",\"code\":\"421\"}";
	char *response1 = "{\"code\":\"421\"}";
	char msg[1024] = {0};

	switch(flag)
	{
		case 0:
		{
			snprintf(msg, sizeof(msg), header, 132);
			len = strlen(msg);
			snprintf(msg + len, sizeof(msg) - len - 1, body, response0);
			break;
		}
		case 1:
		{
			snprintf(msg, sizeof(msg), header, 44);
			len = strlen(msg);
			snprintf(msg + len, sizeof(msg) - len - 1, body, response1);
			break;
		}
		default:
			break;

	}
	send(fd, msg, strlen(msg), 0);
	printf("send(%d): %s\n", strlen(msg), msg);
}

void* data_process(void *p)
{
	int recv_size = 0;
	int buflen = 0;
	int on = 1;
	int clientfd = *(int*)p;
	struct tcp_info info;
	int infolen = sizeof(info);
	char buf[256] = {0};
	
	pthread_detach(pthread_self());
	printf("data_process enter\n");
	printf("read event arrived on %d\n", clientfd);
	
	on = fcntl(clientfd, F_GETFL, 0);
	fcntl(clientfd, F_SETFL, on|O_NONBLOCK);
	//setsockopt(pfdio->newfd, SOL_SOCKET, SO_NONBLOCK, &on, sizeof(on));

	do
	{
		memset(&info, 0, sizeof(info));
		if (!getsockopt(clientfd, IPPROTO_TCP, TCP_INFO, (void*)&info, (socklen_t*)&infolen) 
			&& (TCP_CLOSE == info.tcpi_state || TCP_CLOSE_WAIT == info.tcpi_state || TCP_CLOSING == info.tcpi_state))
		{
			close(clientfd);
			printf("tcp link closed %d because %d\n", clientfd, info.tcpi_state);
			break;
		}
		//printf("tcpinfo state:%d:%d\n", clientfd,info.tcpi_state);

		buflen = sizeof(buf);
		memset(buf, 0, sizeof(buf));
		recv_size = read_line(clientfd,buf,&buflen);
		if (recv_size < 0)
		{
			close(clientfd);
			printf("client disconnect on fd %d!\n", clientfd);
			break;
		}

		if (0 < recv_size)
		{
			printf("size:%d,recv:%s\n",recv_size,buf);
			if (strstr(buf, "productclass"))
			{
				do_write(clientfd, 0);
			}
			else if (strstr(buf, "maxSpeed"))
			{
				do_write(clientfd, 1);
			}
		}

	} while(1);

	printf("data_process out\n");
	pthread_exit(NULL);
}

int init_socket(int port)
{
	int socketfd = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);
	if(socketfd == -1)
	{
		perror("socket error");
		exit(1);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	memset(server_addr.sin_zero,0,sizeof(server_addr.sin_zero));
	int opt = 1;//1代表重用地址
	/* 如果服务器终止后,服务器可以第二次快速启动而不用等待一段时间  */
	int err = setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
	if(err == -1)
	{
		perror("setsockopt reuseaddr error");
		exit(1);
	}

	int ret = bind(socketfd,(struct sockaddr *)(&server_addr),sizeof(server_addr));
	if(ret == -1)
	{
		perror("bind error");
		exit(1);
	}

	ret = listen(socketfd,20);
	if(ret == -1)
	{
		perror("listen error");
		exit(1);
	}
	printf("My Web Server is Ready on %d!\n", socketfd);
	return socketfd;
}

void start_server(int port)
{
	pthread_t tid;
	int i = 0;
	int clientfd = -1;
	int socketfd = -1;
	struct timeval tv;
	
	socklen_t socklen = sizeof(struct sockaddr_in);
	struct sockaddr_in addr;

	socketfd = init_socket(port);

	while(1)
	{
		clientfd = accept(socketfd,(struct sockaddr*)&addr,&socklen);
		if(0 <= clientfd)
		{
			printf("client %s:%d conencted on fd %d.\n", inet_ntoa(addr.sin_addr), htons(addr.sin_port), clientfd);
			pthread_create(&tid, NULL, data_process, &clientfd);
		}
		usleep(100000);
	}
}

int main(int argc,char* argv[])
{
	start_server(8090);
	return 0;
}


