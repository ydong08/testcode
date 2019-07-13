

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/select.h>

const char* addr4 = "192.168.0.166";
const char* addr6 = "fe80::d294:66ff:fe80:9460%eno1";

#define WIFIDOG_PORT  2060
#define READ_BUF_MAX_LEN 128


void* server_thread(void* arg) {
  pthread_detach(pthread_self());
  int fd = *(int*)arg;
  printf("begin recv fd[%d]\n", fd);
  int readbyte = 0;
  char buf[READ_BUF_MAX_LEN];
  while(1) {

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
 
  int fd4 = -1;
  int fd6 = -1;
#if 1
  /* create ipv4 socket */
  fd4 = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC, 0);
  if (fd4 < 0) {
    printf("socket4 fail[%s]\n", strerror(errno));
    //continue;
    goto GEXIT;
  }
  
  struct sockaddr_in si;
  memset(&si, 0, sizeof(si));
  si.sin_family = AF_INET;
  si.sin_port = htons(WIFIDOG_PORT);
  inet_pton(AF_INET, addr4, &si.sin_addr);
  ret = bind(fd4, (struct sockaddr*)&si, sizeof(si));
  if (ret < 0) {
    printf("bind4 fail[%d:%s]\n", errno, strerror(errno));
    close(fd4);
    goto GEXIT;
  }
  
  ret = listen(fd4, 10);
  if (ret < 0) {
    printf("listen4 fail[%s]\n", strerror(errno));
    close(fd4);
    goto GEXIT;
  }
#endif 
 
#if 1
  /* create ipv6 socket */
  fd6 = socket(AF_INET6, SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC, 0);
  if (fd6 < 0) {
    printf("socket6 fail[%s]\n", strerror(errno));
    goto GEXIT;
  }
  setsockopt(fd6, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&ret, sizeof(ret));
  
  struct sockaddr_in6 si6;
  memset(&si6, 0, sizeof(si6));
  si6.sin6_family = AF_INET6;
  si6.sin6_port = htons(WIFIDOG_PORT);
  inet_pton(AF_INET6, addr6, &si6.sin6_addr);
  ret = bind(fd6, (struct sockaddr*)&si6, sizeof(si6));
  if (ret < 0) {
    printf("bind6 fail[%d:%s]\n", errno, strerror(errno));
    close(fd6);
    goto GEXIT;
  }
  
  ret = listen(fd6, 10);
  if (ret < 0) {
    printf("listen6 fail[%s]\n", strerror(errno));
    close(fd6);
    goto GEXIT;
  }
#endif 
  
  if (fd4 == -1)
    fd4 = fd6;
  if (fd6 == -1)
    fd6 = fd4;
    
  /* select */
  int error = 0;
  int num = 0;
  int acceptfd = 0;
  int newfd = 0;
  int cfd = 0;
  int maxfd = fd4 < fd6 ? fd6 + 1 : fd4 + 1;
  fd_set  fds;
  unsigned short proto = 0;
  struct sockaddr_in* addr4 = NULL;
  struct sockaddr_in6* addr6 = NULL;
  struct timeval tv = {.tv_sec = 3, .tv_usec = 1000000};
  pthread_t tid;
  FD_ZERO(&fds);
  FD_SET(fd4, &fds);
  FD_SET(fd6, &fds);
  
  /* begin recv */
  struct sockaddr_storage ss;
  memset(&ss, 0, sizeof(ss));
  socklen_t socklen = sizeof(ss);
  char saddr[INET6_ADDRSTRLEN] = {0};
  
  while(1) {
    FD_ZERO(&fds);
    FD_SET(fd4, &fds);
    FD_SET(fd6, &fds);
    tv.tv_sec = 3;
    tv.tv_usec = 1000000;
    num = select(maxfd, &fds, NULL, NULL, &tv);
    if (num < 0) {
      printf("select fail[%d:%s]\n", errno, strerror(errno));
      sleep(1);
      continue;
    }
    
    for (int i = 0; i < num; ++i) {
      if (FD_ISSET(fd4, &fds)) {
        acceptfd = fd4;
      } else if (FD_ISSET(fd6, &fds)) {
        acceptfd = fd6;
      }
      
      while(1) {
        cfd = accept4(acceptfd, (struct sockaddr*)&ss, &socklen, 0);
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
      } /* while accept */  
    } /* for */  
  } /* while select */
  
GEXIT:
  if (0 < fd4)
    close(fd4);
   if (0 < fd6)
     close(fd6);  
     
  return 0;
}


