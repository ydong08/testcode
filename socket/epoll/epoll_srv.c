
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <net/if.h>
#include <string.h>


#define INVALID_FD -1
#define LEN_EVENT 20
#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define PORTNUMBER 6001

void setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        return;
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        return;
    }
}

void CloseAndDisable(int sockid, struct epoll_event ee)
{
    close(sockid);
    ee.data.fd = -1;
}

/* 通过sockfd获取mac地址 */
int getPeerMacbySocketFd( int sockfd, char *buf, char* localethname )
{
    int ret =0;
    struct arpreq arpreq;
    struct sockaddr_in dstadd_in;
    socklen_t  len = sizeof( struct sockaddr_in );
    memset( &arpreq, 0, sizeof( struct arpreq ));
    memset( &dstadd_in, 0, sizeof( struct sockaddr_in )); 
    if(getpeername(sockfd, (struct sockaddr*)&dstadd_in, &len) < 0 )
    {
        perror( "get peer name wrongn");
        return -1;
    }

    memcpy( ( struct sockaddr_in * )&arpreq.arp_pa, ( struct sockaddr_in * )&dstadd_in, sizeof( struct sockaddr_in ));
    strcpy(arpreq.arp_dev, localethname);
    arpreq.arp_pa.sa_family = AF_INET;
    arpreq.arp_ha.sa_family = AF_UNSPEC;
    if(ioctl(sockfd, SIOCGARP, &arpreq ) < 0 )
    {
        perror( "ioctl SIOCGARP wrong");
        return -1;
    }

    unsigned char* ptr = (unsigned char *)arpreq.arp_ha.sa_data;
    sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
     return ret;
}

/* 通过对端IP获取mac地址 */
int gahGetPeerMacbyIp(char *ipaddr, char* buf, char* localethname)
{
	int     sockfd;
	unsigned char *ptr;
	struct arpreq arpreq;
	struct sockaddr_in *sin;
	struct sockaddr_storage ss;
	char addr[INET_ADDRSTRLEN+1];

	memset(addr, 0, INET_ADDRSTRLEN+1);
	memset(&ss, 0, sizeof(ss));

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
	        perror("socket error");
	        return -1;
	}
	sin = (struct sockaddr_in *) &ss;
	sin->sin_family = AF_INET;
	if (inet_pton(AF_INET, ipaddr, &(sin->sin_addr)) <= 0) {
	        perror("inet_pton error");
	        return -1;
	}
	sin = (struct sockaddr_in *) &arpreq.arp_pa;
	memcpy(sin, &ss, sizeof(struct sockaddr_in));
	strcpy(arpreq.arp_dev, localethname);
	arpreq.arp_ha.sa_family = AF_UNSPEC;
	if (ioctl(sockfd, SIOCGARP, &arpreq) < 0) {
	        perror("ioctl SIOCGARP: ");
	        return -1;
	}
	ptr = (unsigned char *)arpreq.arp_ha.sa_data;
	sprintf(buf,"%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
	return 0;
}

int main()
{
    int i;
	int maxi;
	int listenfd;
	int connfd;
	int sockfd;
	int epfd;
	int nfds;
	int portnumber;
    int recvNum = 0;
    int count = 0;
    int bReadOk = 0;
    char line[MAXLINE];
	char mac[20] = {0};
	char ethname[8] = {"ens33"};
    unsigned char buf[1024] = {0};
    socklen_t clilen;
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;

    //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
    struct epoll_event ev,events[20];

    //生成用于处理accept的epoll专用的文件描述符
    epfd=epoll_create(256);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port=htons(PORTNUMBER);

    // bind and listen
    bind(listenfd,(struct sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);

    //设置与要处理的事件相关的文件描述符
    ev.data.fd=listenfd;
    //设置要处理的事件类型
    ev.events=EPOLLIN|EPOLLET;
    //ev.events=EPOLLIN;

    //注册epoll事件
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

    for ( ; ; )
    {
        //等待epoll事件的发生
        nfds=epoll_wait(epfd,events,20,-1);

        //处理所发生的所有事件
        for(i=0;i<nfds;++i)
        {
            //如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。
            if(events[i].data.fd==listenfd)
            {
                connfd = accept(listenfd,(struct sockaddr *)&clientaddr, &clilen);
                if(connfd < 0){
                    perror("accept");
                  //  return (1);
				  continue;
                }

                char *str = inet_ntoa(clientaddr.sin_addr);

                //设置用于读操作的文件描述符
                setnonblocking(connfd);
                ev.data.fd=connfd;
                //设置用于注测的读操作事件
                ev.events=EPOLLIN | EPOLLET;

                //注册ev
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
            }
            else if(events[i].events & EPOLLIN)
            {
                //如果是已经连接的用户，并且收到数据，那么进行读入。
                if ( (sockfd = events[i].data.fd) < 0)
                    continue;

				getPeerMacbySocketFd(sockfd, mac, ethname);
				printf("mac:%s\n", mac);
                // 确保sockfd是nonblocking的
                recvNum = recv(sockfd, buf, 1024, 0);
				for (int i = 0; i < recvNum; ++i)
				{
					printf("%02x ", buf[i]);
				}
				printf("\n");
				// if (recvNum > 0)
				// {
				// 	send(sockfd, buf, recvNum, 0);
				// }
            }
        }
    }
    return 0;
}

