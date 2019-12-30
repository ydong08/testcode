#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define INVALID_FD -1
#define LEN_EVENT 20

// 创建服务端套接字
int createServerSock()
{
	// 创建套接字
	int sockfd_server = socket(AF_INET, SOCK_STREAM, 0);
	
	if (INVALID_FD == sockfd_server)
	{
		return INVALID_FD;
	}
	
	struct sockaddr_in _sockaddr;
	memset(&_sockaddr, 0, sizeof(_sockaddr));
	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_port = htons(9900);
	_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// 绑定套接字
	bind(sockfd_server, (sockaddr *)&_sockaddr, sizeof(struct sockaddr_in));
	
	// 监听
	listen(sockfd_server, 100);
	
	return sockfd_server;
}

// 设置套接字
void setsocketfd(int socketfd)
{
	int flags = fcntl(socketfd, F_GETFL);
	fcntl(socketfd, F_SETFL, flags | O_NONBLOCK);
}


int main()
{
	// 创建套接字
	int socket_server = createServerSock();
	if (INVALID_FD == socket_server)
	{
		printf("create server socket error.\n");
		return EXIT_FAILURE;
	}
	
	int fd_epoll = epoll_create(256); // 创建epoll描述符
	
	if (INVALID_FD == fd_epoll)
	{
		printf("create epoll error \n");
		return EXIT_FAILURE;
	}
	
	// 注册epoll事件
	struct epoll_event event, events[LEN_EVENT];
	event.data.fd = socket_server;
	event.events = EPOLLIN | EPOLLET;
	
	int ret = epoll_ctl(fd_epoll, EPOLL_CTL_ADD, socket_server, &event);
		
	if  (ret == INVALID_FD)
	{
		printf("注册epoll事件失败..\n");
		return EXIT_FAILURE;
	}
	
	printf("注册epoll事件成功..\n");
	int nfds = -1;
	for ( ; ; )
	{
		nfds=epoll_wait(fd_epoll, events, LEN_EVENT, -1);
		printf("新的连接进入....");
		for (int i = 0; i < nfds; ++i)
		{
			//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。
			if(events[i].data.fd == socket_server)
			{
				printf("新的连接进入....");
			}
			
		}
	}
	
	return EXIT_SUCCESS;
}
