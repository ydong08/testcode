
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
void *func(void *arg);
void *func2(void *arg);
int main()
{
  int res;
  pthread_t a_thread;
  void *thread_res;
  res = pthread_create(&a_thread,NULL,func2,(void*)0);
  if(res != 0)
  {
    perror("Fail to create a new thread");
    exit(EXIT_FAILURE);
  }
  printf("Waiting  for the thread to finish...thread_res -->%0x\n",thread_res);
  res = pthread_join(a_thread,(void*)&thread_res);
  if(res != 0)
  {
    perror("thread join error");
    exit(EXIT_FAILURE);
  }
  printf("pointer return from thread is thread_res = %0x\n",thread_res);
  printf("thread finished! return value:%d\n",thread_res);
  exit(EXIT_SUCCESS);
}


void *func(void *arg)
{
  sleep(3);
  pthread_exit("Thread funtion finished!\n");
}

void * func2(void* p) {
    usleep(3000000);
    pthread_exit((void*)2);
}
