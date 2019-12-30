

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <ipv6.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>



#define ENABLE_IPV6
//#define ENABLE_IPV4
//#define ENABLE_AUTO

#define WIFIDOG_IFNAME  "breth8"
#if defined(ENABLE_IPV4)
const char* addr = "192.168.200.128";
#elif defined(ENABLE_IPV6)
const char* addr = "fe80::d294:66ff:fe80:9460%ens33";
#elif defined(ENABLE_AUTO)
const char* addr = NULL;
#endif 

#define WIFIDOG_PORT  "2060"
#define READ_BUF_MAX_LEN 128


void* server_thread(void* arg) {
  pthread_detach(pthread_self());
  int fd = *(int*)arg;
  printf("fd[%d]\n", fd);
  int readbyte = 0;
  char buf[READ_BUF_MAX_LEN];
  while(1) {
    printf("begin recv\n");
    memset(buf, 0, sizeof(buf));
    readbyte = recv(fd, buf, sizeof(buf), 0);
    printf("readbyte[%d], errno[%d]\n", readbyte, errno);
    if (readbyte < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
      printf("break recv");
      break;
    }
    if (0 < readbyte)
      printf("\tread[%s]\n", buf);
    sleep(1);
  }
  
  close(fd);
  printf("end recv");
  pthread_exit(NULL);
  
}

int main(int argc, char** argv) {

  /* check ifs */
  struct ifaddrs *ifap;
  const struct ifaddrs *it;
  char host[NI_MAXHOST] = {0};
  int ret = getifaddrs(&ifap);
  if (ret < 0) {
    printf("getifaddrs fail\n");
  }
  
  for (it = ifap; it != NULL; it=it->ifa_next) {
    if (!it->ifa_addr)
      continue;
    
    ret = it->ifa_addr->sa_family;
    printf("%-8s %s(%d)\n", it->ifa_name, 
          ret == AF_PACKET ? "AF_PACKET" : (ret == AF_INET) ? "AF_INET" : (ret == AF_INET6) ? "AF_INET6" : "???", 
          ret);
          
    if (ret == AF_INET || ret == AF_INET6) {
      ret = getnameinfo(it->ifa_addr, 
                        ret == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                        host, NI_MAXHOST,
                        NULL, 0, NI_NUMERICHOST); // NI_NUMERICHOST
      if (!ret) 
        printf("\t[%s]\n", host);
      
    }
  }
  
  /* getaddrinfo */
  struct addrinfo hint;
  struct addrinfo *res = NULL;
  struct addrinfo *pit;
  memset(&hint, 0, sizeof(hint));
  hint.ai_family = PF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE|AI_ADDRCONFIG;
  char saddr[INET6_ADDRSTRLEN] = {0};
  ret = getaddrinfo(addr, WIFIDOG_PORT, &hint, &res); 
  if (ret < 0 || res == NULL) {
    printf("getaddrinfo error: %s\n", gai_strerror(ret));
    exit(EXIT_FAILURE);
  } 
  
  for (pit = res; pit != NULL; pit = pit->ai_next) {
    printf("family[%d], address len[%d]\n", pit->ai_family, pit->ai_addrlen);
  }
  
  if (!res) {
    printf("addrinfo null\n");
    goto GEXIT;
  }
  
  /* create socket */
  int fd = socket(res->ai_family, SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC, 0);
  if (fd < 0) {
    printf("socket fail[%s]\n", strerror(errno));
    //continue;
    goto GEXIT;
  }
  
  const char* dst = NULL;
  if (res->ai_family == AF_INET) {
    dst = inet_ntop(res->ai_family, &((struct sockaddr_in*)(res->ai_addr))->sin_addr, saddr, INET6_ADDRSTRLEN);
  } else if (res->ai_family == AF_INET6) { 
    dst = inet_ntop(res->ai_family, &((struct sockaddr_in6*)(res->ai_addr))->sin6_addr, saddr, INET6_ADDRSTRLEN);
  }
  
  if (!dst)
    printf("inet_ntop fail[%s]\n", strerror(errno));
  else
    printf("family[%d], bind address[%s]\n", res->ai_family, saddr);
  
  ret = bind(fd, res->ai_addr, (socklen_t)res->ai_addrlen);
  if (ret < 0) {
    printf("bind fail[%d:%s]\n", errno, strerror(errno));
    close(fd);
    //continue;
    goto GEXIT;
  }
  
  ret = listen(fd, 10);
  if (ret < 0) {
    printf("listen fail[%s]\n", strerror(errno));
    close(fd);
    goto GEXIT;
  }
  
  int error = 0;
  pthread_t tid;
  unsigned short proto = 0;
  
  struct sockaddr_in* addr4 = NULL;
  struct sockaddr_in6* addr6 = NULL;
  
  struct sockaddr_storage ss;
  memset(&ss, 0, sizeof(ss));
  socklen_t socklen = sizeof(ss);
  
  int newfd = 0;
  while(1) {
    int cfd = accept4(fd, (struct sockaddr*)&ss, &socklen, 0);
    if (cfd < 0) {
      error = errno;
      if (error == EAGAIN) {
        sleep(1);
        continue;
      } else {
        printf("accept fail[%d]\n", error);
        break;
      }
        
    }
    
    proto = *(unsigned short*)&ss;
    printf("new connect type[%hd]\n", proto);
    memset(saddr, 0, sizeof(saddr));
    if (proto == AF_INET) {
      addr4 = (struct sockaddr_in*)&ss;
      inet_ntop(AF_INET, &(addr4->sin_addr), saddr, INET6_ADDRSTRLEN);
      printf("proto[%d] addr[%s]\n", addr4->sin_family, saddr);
    } else if (proto == AF_INET6) {
      addr6 = (struct sockaddr_in6*)&ss;
      inet_ntop(AF_INET6, &(addr6->sin6_addr), saddr, INET6_ADDRSTRLEN);
      printf("proto[%d] addr[%s]\n", addr6->sin6_family, saddr);
    }
    
    newfd = cfd;
    printf("fd[%d]\n", cfd);
    ret = pthread_create(&tid, NULL, server_thread, &newfd);
    if (ret < 0) {
      printf("create therad fail: %d\n", ret);
    }
  }
  
GEXIT:
  close(fd);
  freeaddrinfo(res);
  return 0;
}


