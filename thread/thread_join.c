
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include <locale.h>

/* KP1 ~ KP5 */
void *func(void *arg);
void *func2(void *arg);
int main()
{
  int res;
  pthread_t a_thread;
  int *thread_res;
  res = pthread_create(&a_thread,NULL,func,(void*)0);
  if(res != 0)
  {
    perror("Fail to create a new thread");
    exit(EXIT_FAILURE);
  }
  // KP5: printf #, 和浮点数配合使用时会输出精度尾零,即使要打印的数没有小数点(如:987)
  float ft = 987;
  printf("ft = %#f\n", ft);  // 987.000000
  // KP3: printf #, #和前置补充的0冲突,不会有前置补充的0显示
  printf("Waiting  for the thread to finish...thread_res -->%0#x\n",thread_res); // 0x400710
  res = pthread_join(a_thread,(void*)&thread_res);
  if(res != 0)
  {
    perror("thread join error");
    exit(EXIT_FAILURE);
  }
  
  printf("pointer return from thread is thread_res = %0#x\n",thread_res);
  printf("thread finished! return value:%d\n",thread_res);
  exit(EXIT_SUCCESS);
}


void *func(void *arg)
{
  sleep(3);
  // KP4: printf '
  float num = 1234567.89;
  printf("num = %'.2f\n", num);

  char localbuf[16];
  struct lconv* lcv = NULL; 
  char* local = setlocale(LC_NUMERIC, "");
  printf("default locale: %s\n", local);
  memcpy(localbuf, local, strlen(local));

  lcv = localeconv();
  printf("THOUSANDS SEP: %s\n", lcv->thousands_sep);
  printf("num = %'.2f\n", num);

  setlocale(LC_NUMERIC, "nl_NL");
  lcv = localeconv();
  printf("THOUSANDS SEP: %s\n", lcv->thousands_sep);
  printf("num = %'.2f\n", num);

  setlocale(LC_NUMERIC, "da_DK");
  lcv = localeconv();
  printf("THOUSANDS SEP: %s\n", lcv->thousands_sep);
  printf("num = %'.2f\n", num);

  setlocale(LC_NUMERIC, localbuf);
  lcv = localeconv();
  printf("THOUSANDS SEP: %s\n", lcv->thousands_sep);


  // KP1: exit status
  pthread_exit("Thread funtion finished!\n");
}

void * func2(void* p) {
  usleep(3000000);
  // KP5: printf *
  int width = 8;
  int num = 123456;
  printf("num = %*d\n", width, num);
  // KP2: exit status
  pthread_exit((void*)2);
}
