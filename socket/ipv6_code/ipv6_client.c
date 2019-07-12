

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


//#define ENABLE_IPV6
#define ENABLE_IPV4
//#define ENABLE_AUTO

#if defined(ENABLE_IPV4)
const char* addr = "192.168.200.128";
#elif defined(ENABLE_IPV6)
const char* addr = "fe80::20c:29ff:fe32:b303%ens33";
#elif defined(ENABLE_AUTO)
const char* addr = NULL;
#endif 

#define WIFIDOG_PORT  "2060"
#define WRITE_BUF_MAX_LEN 128
#define WRITE_CONTENT "abcdefghijklmnopqrstuvwxyz"


void* client_thread(void* arg) {
  //pthread_detach(pthread_self());
  int fd = *(int*)arg;
  int writebyte = 0;
  char buf[WRITE_BUF_MAX_LEN] = {0};
  while(1) {
    memcpy(buf, WRITE_CONTENT, strlen(WRITE_CONTENT));
    writebyte = send(fd, buf, strlen(buf), 0);
    if (writebyte < 0 && (errno != EAGAIN || errno != EWOULDBLOCK))
      break;
    printf("write len[%d]\n", writebyte);
    sleep(1);
  }
  
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
      if (!ret) {
        printf("\t[%s]\n", host);
      }
    }
  }
  
 
  /* getaddrinfo */
  struct addrinfo hint;
  struct addrinfo *res = NULL;
  memset(&hint, 0, sizeof(hint));
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_ADDRCONFIG;
  ret = getaddrinfo(addr, WIFIDOG_PORT, &hint, &res); 
  if (ret < 0 || res == NULL) {
    printf("getaddrinfo error: %s\n", gai_strerror(ret));
    exit(EXIT_FAILURE);
  } 


#if 0 
  struct sockaddr *sa = NULL;
  unsigned short family = 0;
  socklen_t len = 0;
#if defined(ENABLE_IPV6)  
  struct sockaddr_in6 si6;
  memset(&si6, 0, sizeof(si6)); 
  family = AF_INET6; 
  si6.sin6_family = AF_INET6;
  si6.sin6_port = htons(WIFIDOG_PORT);
  inet_pton(AF_INET6, addr, &si6.sin6_addr);
  sa = (struct sockaddr*)&si6;
  len = sizeof(si6);
#endif

#if defined(ENABLE_IPV4)
  struct sockaddr_in si;
  memset(&si, 0, sizeof(si));
  family = AF_INET6;
  si.sin_family = AF_INET;
  si.sin_port = htons(WIFIDOG_PORT);
  inet_pton(AF_INET, addr, &si.sin_addr);
  sa = (struct sockaddr*)&si;
  len = sizeof(si);
#endif
#endif
  
  /* create socket */
  int fd = socket(res->ai_family, SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC, 0);
  if (fd < 0) {
    printf("socket fail[%s]\n", strerror(errno));
    return -1;
  }
  
  do
  {
    ret = connect(fd, res->ai_addr, (socklen_t)res->ai_addrlen);
    if (ret < 0 && errno != EINPROGRESS) {
      printf("connect fail[%d:%s]\n", errno, strerror(errno));
      close(fd);
      return -1;
    }
    
    if (!ret)
      break;
      
  } while(1);
  
  pthread_t tid;
  ret = pthread_create(&tid, NULL, client_thread, &fd);
  if (ret < 0) {
    printf("create therad fail: %d\n", ret);
  }

  pthread_join(tid, NULL);
  
  close(fd);
  return 0;
}


