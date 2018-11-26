
#define HOST_PUBLIC_IP        "106.14.143.134"
#define HOST_PRIVATE_IP       "172.19.16.173"
#define HOST_PORT             (3389)

#define ENABLE_SERVER
//#define ENABLE_CLIENT
/******* 服务器程序  (server.c) ************/ 
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

#ifdef ENABLE_SERVER
int main(int argc, char *argv[]) { 
    int sockfd,new_fd;         
    struct sockaddr_in server_addr;         
    struct sockaddr_in client_addr;         
    int sin_size,portnumber; 
    char hello[]="Hello! Are You Fine?\n"; 
    char world[64] = {0};
    int retval = 0; 
    int len, error = 0;
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
        if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1) 
        { 
            fprintf(stderr,"Accept error:%s\n\a",strerror(errno));                         
            exit(1);                 
        } 
         
        fprintf(stderr,"Server get connection from %s\n", inet_ntoa(client_addr.sin_addr)); 
        do
        {
            error = 0;
            memset(&world, 0x00, sizeof(world));
            getsockopt(new_fd, SOL_SOCKET, SO_ERROR, &error, &len);
            retval = recv(new_fd, world, sizeof(world), 0);
            if (0 == retval) {
                printf("close newfd: %d\n", error); 
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
    inet_pton(AF_INET, HOST_PUBLIC_IP, &server_addr.sin_addr.s_addr);  

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
        if (nbytes < 0)
            break;
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