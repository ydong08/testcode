
/******* 服务器程序  (server.c) ************/ 
#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <unistd.h> 
#include <netdb.h> 
#include <fcntl.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <linux/tcp.h>

#define HOST_PUBLIC_IP        "192.168.27.129"
#define HOST_PRIVATE_IP       "172.19.16.173"
#define HOST_PORT             (6001)

#define ENABLE_CLIENT
#ifdef ENABLE_SERVER
int main(int argc, char *argv[]) { 
    int sockfd,new_fd;         
    struct sockaddr_in server_addr;         
    struct sockaddr_in client_addr;         
    int sin_size,portnumber; 
    char world[64] = {0};
    int retval = 0; 
    int len;
    struct tcp_info info;
    int flags = 0;
    int reuseaddr = 1;
#if 0      
    if(argc!=2)        
    { 
       fprintf(stderr,"Usage:%s portnumber\a\n",argv[0]);                 
        exit(1);         
    } 
    if((portnumber=atoi(argv[1]))<0)         
    { 
        fprintf(stderr,"Usage:%s portnumber\a\n",argv[0]);                 
        exit(1);         
    } 
#endif
    /* 服务器端开始建立socket描述符 */        
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)           
    { 
         fprintf(stderr,"Socket error:%s\n\a",strerror(errno));                 
         exit(1);         
    } 
    // reuseaddr
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&reuseaddr, sizeof(reuseaddr));

    // keeplive
    int keepAlive = 1; // 开启keepalive属性
    int keepIdle = 30; // 如该连接在60秒内没有任何数据往来,则进行探测 
    int keepInterval = 5; // 探测时发包的时间间隔为5 秒
    int keepCount = 3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后2次的不再发.

    /* 服务器端填充 sockaddr结构  */  
    memset(&server_addr, 0, sizeof(struct sockaddr_in)); 
    portnumber = HOST_PORT;
    server_addr.sin_family=AF_INET; 
    //server_addr.sin_addr.s_addr=htonl(INADDR_ANY);         
    server_addr.sin_port=htons(portnumber);   
    inet_pton(AF_INET, HOST_PRIVATE_IP, &server_addr.sin_addr.s_addr);      
    /* 捆绑sockfd描述符  */  
    if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1) 
    { 
         fprintf(stderr,"Bind error:%s\n\a",strerror(errno));                 
         exit(1);         
    } 
    /* 监听sockfd描述符  */ 
    if(listen(sockfd,5)==-1)         
   { 
       fprintf(stderr,"Listen error:%s\n\a",strerror(errno));                 
       exit(1);        
   }        
   while(1) 
   { 
        /* 服务器阻塞,直到客户程序建立连接  */                 
        sin_size=sizeof(struct sockaddr_in);  
        printf("begin to accept\n");              
        if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),(socklen_t*)&sin_size))==-1) 
        { 
            fprintf(stderr,"Accept error:%s\n\a",strerror(errno));                         
            exit(1);                 
        } 
        flags = fcntl(new_fd, F_GETFL, 0);
        fcntl(new_fd, F_SETFL, flags|O_NONBLOCK);

        setsockopt(new_fd, SOL_SOCKET, SO_KEEPALIVE, (int[]){1}, sizeof(int));
        setsockopt(new_fd, IPPROTO_TCP, TCP_KEEPIDLE, (int[]){30}, sizeof(int));
        setsockopt(new_fd, IPPROTO_TCP, TCP_KEEPINTVL, (int[]){5}, sizeof(int));
        setsockopt(new_fd, IPPROTO_TCP, TCP_KEEPCNT, (int[]){3}, sizeof(int));
        fprintf(stderr,"Server get connection from %s\n", inet_ntoa(client_addr.sin_addr));

        do
        {
            memset(&world, 0x00, sizeof(world));
            // len = sizeof(info);
            // memset(&info, 0x00, sizeof(info));
            // getsockopt(new_fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t*)&len);
            // printf("tcpi_state: %d\n", info.tcpi_state);
            // if (1 != info.tcpi_state) {
            //     printf("close newfd: %d\n", errno); 
            //     close(new_fd);
            //     break;
            // }

            retval = recv(new_fd, world, sizeof(world), 0);
            if (retval < 0 && ETIMEDOUT == errno) {
                printf("close newfd\n"); 
                close(new_fd);
                break;
            }
            if (0 < retval){
                printf("Recv: %d|%s\n", retval, world);
                if (0 < (retval = send(new_fd, world, retval, 0)))
                    printf("Send: %d|%s\n", retval, world);
            }
            usleep(1000000);
        } while(1);         
    }   
    close(sockfd);        
    exit(0); 
}
#endif 



#ifdef ENABLE_CLIENT
/******* 客户端程序  client.c ************/ 
#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <unistd.h> 
#include <netdb.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <sys/types.h> 
#include <arpa/inet.h> 

int main(int argc, char *argv[]) 
{ 
    int sockfd; 
    char buffer[1024]; 
    struct sockaddr_in server_addr;         
    struct hostent *host;         
    int portnumber,nbytes;
#if 0     
    if(argc!=3)         
    { 
        fprintf(stderr,"Usage:%s hostname portnumber\a\n",argv[0]);                 
        exit(1);         
    } 
    if((host=gethostbyname(argv[1]))==NULL)         
    { 
        fprintf(stderr,"Gethostname error\n");                 
        exit(1);         
    } 
    if((portnumber=atoi(argv[2]))<0)        
    { 
            fprintf(stderr,"Usage:%s hostname portnumber\a\n",argv[0]);                 
        exit(1);         
    } 
#endif    
    /* 客户程序开始建立 sockfd描述符  */ 
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)        
    { 
        fprintf(stderr,"Socket Error:%s\a\n",strerror(errno)); 
        exit(1);        
    } 
    /* 客户程序填充服务端的资料 */  
    portnumber = HOST_PORT;       
    bzero(&server_addr,sizeof(server_addr));         
    server_addr.sin_family=AF_INET; 
    server_addr.sin_port=htons(portnumber); 
    //server_addr.sin_addr=*((struct in_addr *)host->h_addr);  
    inet_pton(AF_INET, HOST_PUBLIC_IP, &server_addr.sin_addr);  

    /* 客户程序发起连接请求*/  
    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)        
    { 
        fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));                 
        exit(1);         
    } 
    /* 连接成功了*/ 
    char buf[] = "abcdefghijklmnopqrst";
    do
    {
        nbytes = send(sockfd, buf, sizeof(buf), 0);
		printf("send %d.\n", nbytes);
        //if (nbytes < 0)
         //   break;
        usleep(1000000);
    } while(1);

#if 0
    if((nbytes=read(sockfd,buffer,1024))==-1)  //if((nbytes=recv(sockfd,buffer,1024))==-1)       
    { 
        fprintf(stderr,"Read Error:%s\n",strerror(errno));                
        //exit(1);         
    } 
    buffer[nbytes]='\0'; 
    printf("I have received:%s\n",buffer);         
    /* 结束通讯     */   
#endif      
    close(sockfd);         
    exit(0); 
}

#endif
