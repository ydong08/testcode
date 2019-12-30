#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/un.h>


#include <event2/event.h>

#define EVENT_METHOD_EPOLL "epoll"
#define EVENT_METHOD_POLL  "poll"
#define EVENT_METHOD_SELECT "select"

#define EVENT_FILE_NAME     "tmpsock.sock"
#define EVENT_FILE_CONTENT  "abcdefghijklmnopqrstuvwxyz"


void* write_thread(void* arg)
{
  pthread_detach(pthread_self());
  int len = 0;
  int count = 50;
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0) {
    printf("socket fail[%d]\n", errno);
    pthread_exit(NULL);
  }
  
  struct sockaddr_un sa_un;
  memset(&sa_un, 0, sizeof(sa_un));
  sa_un.sun_family = AF_UNIX;
  memcpy(sa_un.sun_path, EVENT_FILE_NAME, strlen(EVENT_FILE_NAME));
  len = bind(fd, (struct sockaddr*)&sa_un, sizeof(sa_un.sun_family)+strlen(sa_un.sun_path));
  if (len < 0) {
    printf("bind socket fail[%d]\n", errno);
    goto SEXIT;
  }
  
  len = listen(fd, 5);
  if (len < 0) {
    printf("listen socket fail[%d]\n", errno);
    goto SEXIT;
  }
  
  *(int*)arg = 1;
  int cfd = accept(fd, (struct sockaddr*)&sa_un, &len);
  if (0 < cfd) {
    while(0 < --count)
    {
      write(cfd, EVENT_FILE_CONTENT, strlen(EVENT_FILE_CONTENT)+1);
      //sleep(1);
    }
    close(cfd);
  }

SEXIT:
  *(int*)arg = 2;
  close(fd);
  sleep(1);
  pthread_exit(NULL);
}

void create_write_thread(void* arg)
{
  pthread_t tid;
  int ret = pthread_create(&tid, NULL, write_thread, arg);
  if (ret < 0) {
    printf("create thread fail[%d]\n", ret);
  }
}

void read_print(evutil_socket_t fd)
{
  if (fd < 0) {
    printf("fd[%d] invalid\n", fd);
    return ;
  }
  
  char buf[128] = {0};
  int ret = read(fd, buf, sizeof(buf));
  if (0 < ret) {
    printf("event read: %s\n", buf);
  } else {
    printf("event fail[%d]\n", errno);
    close(fd);
  }
  return ;

}

void event_callback(evutil_socket_t fd, short events, void *arg)
{
  if (fd < 0)
    printf("invoke event [timer]\n");
  else if (fd == 1)
    printf("invoke event [signal]\n");
  else if (1 < fd) {
    printf("invoke event [socket/IO]\n");
    read_print(fd);
  }
  
  return ;
}

int main(int argc, char** argv) {

  int ret = -1;
  struct event* ev = NULL;
  struct event_config* cfg = NULL;
  struct event_base* evb = NULL;
  
	/* 开启debug模式 */
	event_enable_debug_mode();
  if (argc < 2) {
    printf("config noexist\n");
  }
  
  /* 创建reactor实例 event_base_new_with_config() or event_base_new()*/
  if (2 <= argc && argv[1]) {
    struct event_config* cfg = event_config_new();
    if (!cfg) {
      printf("event_config_new fail\n");
      exit(EXIT_FAILURE);
    } 
    /* 排除不符合的method */
    ret = event_config_avoid_method(cfg, EVENT_METHOD_POLL);
    if (ret < 0) {
      printf("event_config_avoid_method [%s] fail\n", EVENT_METHOD_POLL);
    } else {
      printf("event_config_avoid_method [%s] success\n", EVENT_METHOD_POLL);
    }
    /* 设置config的feature */
    ret = event_config_require_features(cfg, EV_FEATURE_ET);
    evb = event_base_new_with_config(cfg);
    if (!evb) {
      event_config_require_features(cfg, 0);
      evb = event_base_new_with_config(cfg);
    }
  } else {
    evb = event_base_new();	
  }
  
  if (!evb) {
  		printf("event base new fail\n");
      exit(EXIT_FAILURE);
  }
  
	printf("event base features: %#X\n", event_base_get_features(evb));
	printf("kernel event notification method: %s\n", event_base_get_method(evb));
	const char** methods = event_get_supported_methods();
	if (methods) {
		for (int i = 0; methods[i] != NULL; ++i)
			printf(" -libevent event notification method: %s\n", methods[i]);
	}

  /* 设置监听事件 */
  ///*signal事件*/
#if defined(ENABLE_SIGNAL_EVENT) 
  /* If events contains EV_SIGNAL, then fd is a signal number to wait for*/
  ev = event_new(evb, 1, EV_SIGNAL, event_callback, /*event_self_cbarg()*/ NULL);
#endif /* ENABLE_SIGNAL_EVENT */  

  ///*timer事件*/
#if defined(ENABLE_TIMER_EVENT) 
  ev = event_new(evb, -1, 0, event_callback, /*event_self_cbarg()*/ NULL);
#endif /* ENABLE_TIMER_EVENT */

  ///*socket事件*/
#if defined(ENABLE_SOCKET_EVENT)
  /* fd event */
  int status = 0;
  create_write_thread(&status);
  
  while(1) {
    if (1 == status) break;
    if (2 == status) goto GEXIT;
  }
  printf("server listen status[%d]\n", status);

  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0) {
    printf("create socket fail[%d]\n", errno);
  }
  
  struct sockaddr_un sa_un;
  memset(&sa_un, 0, sizeof(sa_un));
  sa_un.sun_family = AF_UNIX;
  memcpy(sa_un.sun_path, EVENT_FILE_NAME, strlen(EVENT_FILE_NAME));
  ret = connect(fd, (struct sockaddr*)&sa_un, sizeof(sa_un.sun_family)+strlen(sa_un.sun_path));
  if (ret < 0) {
    printf("connect fail[%d]\n", errno);
    goto GEXIT;
  }
  
  ev = event_new(evb, fd, EV_WRITE, event_callback, /*event_self_cbarg()*/ NULL);
  if (!ev) {
    printf("event new fail\n");
    goto GEXIT;
  }
#endif /* ENABLE_SOCKET_EVENT */

  /* 将event 加入监控list */
  ret = event_add(ev, 0);
  if (ret < 0) {
    printf("event add fail\n");
    goto GEXIT;
  }
  
	/* 开启event监听 event_base_loop() */
	//ret = event_base_dispatch(evb);
  ret = event_base_loop(evb, EVLOOP_NO_EXIT_ON_EMPTY);
	if (ret < 0)
		printf("event base dispatch fail\n");
	else if (0 < ret)
		printf("no active event\n");
	
	struct timeval tv = {.tv_sec = 100, .tv_usec = 1000000};
	//event_base_loopexit(evb, &tv);
  //usleep(100000000);

GEXIT:  
  if (cfg)
    event_config_free(cfg);
  if (ev)
    event_free(ev);
    
  /* 检测event loop 退出原因 */  
  if (event_base_got_exit(evb))
    printf("event base loop [exit] called\n");
  if (event_base_got_break(evb))
    printf("event base loop [break] called\n");
    
	event_base_free(evb);
  unlink(EVENT_FILE_NAME);
  
	return 0;
}




