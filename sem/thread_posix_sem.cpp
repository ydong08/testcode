
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <semaphore.h>

#define THREAD_NUM     100
#define SEM_PATH_NAME  "mysem"
static int num = 1000;

void* writenum(void* p) {
  // pthread_detach(pthread_self());
  sem_wait((sem_t*)p);
  num += 10;
  printf("%d thread write num: %d\n", syscall(SYS_gettid), num);
  sem_post((sem_t*)p);
  pthread_exit(NULL);
}


void* readnum(void* p) {
  // pthread_detach(pthread_self());
  sem_wait((sem_t*)p);
  printf("%d thread read  num: %d\n", syscall(SYS_gettid), num);
  sem_post((sem_t*)p);
  pthread_exit(NULL);
}


int main() {
  int retval = 0;
  pthread_t wtid[THREAD_NUM] = {0};
  pthread_t rtid[THREAD_NUM] = {0};
  sem_t* sm = sem_open(SEM_PATH_NAME, O_CREAT|O_EXCL, 0664, 1)
  if (SEM_FAILED == sm) {
    printf("create sem fail: %d\n", errno);
    return 1;
  }

  for (int i = 0; i < THREAD_NUM; ++i) {
    retval = pthread_create(&wtid[i], NULL, writenum, sm);
    if (retval < 0) {
      printf("create %dth wthread fail\n", i);
    }
  }

  for (int i = 0; i < THREAD_NUM; ++i) {
    retval = pthread_create(&rtid[i], NULL, readnum, sm);
    if (retval < 0) {
      printf("create %dth wthread fail\n", i);
    }
  }

  for (int i = 0; i < THREAD_NUM; ++i) {
    pthread_join(wtid[i], NULL);
    pthread_join(rtid[i], NULL);
  }

  sem_close(sm);

  return 0;

}