

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <bits/siginfo.h>

// struct timespec {
//   time_t tv_sec;                /* Seconds */
//   long   tv_nsec;               /* Nanoseconds */
// };

// struct itimerspec {
//   struct timespec it_interval;  /* Timer interval */
//   struct timespec it_value;     /* Initial expiration */
// };

// union sigval {          /* Data passed with notification */
//     int     sival_int;         /* Integer value */
//     void   *sival_ptr;         /* Pointer value */
// };

// struct sigevent {
//     int          sigev_notify; /* Notification method */
//     int          sigev_signo;  /* Notification signal */
//     union sigval sigev_value;  /* Data passed with notification */
//     void         (*sigev_notify_function) (union sigval);
//                  /* Function used for thread notification (SIGEV_THREAD) */
//     void         *sigev_notify_attributes;
//                  /* Attributes for notification thread (SIGEV_THREAD) */
//     pid_t        sigev_notify_thread_id;
//                  /* ID of thread to signal (SIGEV_THREAD_ID) */
// };

int sigsum = 10;

void sigact(int signo, siginfo_t* siginfo, void* p) {
  if (SIGRTMIN+10 == signo) {
    --sigsum;  
    printf("recv signal event: %d\n", sigsum);
  }
}

int main() {
  // 1. create timer  
  timer_t trt;
  sigevent_t sgt;
  memset(&sgt, 0x00, sizeof(sgt));
  sgt.sigev_notify = SIGEV_SIGNAL;
  sgt.sigev_signo = SIGRTMIN+10;
  sgt.sigev_value.sival_int = 0x1234; // data pass
  sgt.sigev_notify_function = NULL;
  sgt.sigev_notify_attributes = NULL;
  sgt.sigev_notify_thread_id = 0;
  int retval = timer_create(CLOCK_MONOTONIC, &sgt, &trt);
  if (retval < 0) {
    perror("timer_create");
    return retval;
  }

  // 2. register signal function
  /*
    struct sigaction {
      union {
        __sighandler_t _sa_handler;
        void (*_sa_sigaction)(int, struct siginfo *, void *);
      } _u;
      sigset_t sa_mask;
      unsigned long sa_flags;
      void (*sa_restorer)(void);
    };
  */
  sigset_t sigset;
  sigemptyset(&sigset);
  struct sigaction siga;
  struct sigaction sigao;
  memset(&siga, 0x00, sizeof(siga));
  memset(&sigao, 0x00, sizeof(sigao));
  siga.sa_sigaction = sigact;
  siga.sa_mask = sigset;
  siga.sa_flags = SA_RESTART|SA_SIGINFO;
  retval = sigaction(SIGRTMIN+10, &siga, &sigao);
  if (retval < 0) {
    perror("sigaction");
  }

  // 3. start timer
  struct itimerspec its, oits, nits;
  memset(&its, 0x00, sizeof(its));
  memset(&oits, 0x00, sizeof(oits));
  memset(&nits, 0x00, sizeof(nits));
  its.it_interval.tv_sec = 2;
  its.it_interval.tv_nsec = 1000000;
  its.it_value.tv_sec = 5;
  its.it_value.tv_nsec = 1000000;
  retval = timer_settime(trt, 0, &its, &oits);
  if (retval < 0) {
    perror("timer_settime");
    return retval;
  }

  printf("first settime: value: %ldus, interval: %ldus\n",
        oits.it_value.tv_sec*1000000 + oits.it_value.tv_nsec/1000,
        oits.it_interval.tv_sec*1000000 + oits.it_interval.tv_nsec/1000);

  retval = timer_gettime(trt, &nits);  
  if (retval < 0) {
    perror("timer_gettime");
  }
  printf("first gettime: value: %ldus, interval: %ldus\n",
        nits.it_value.tv_sec*1000000 + nits.it_value.tv_nsec/1000,
        nits.it_interval.tv_sec*1000000 + nits.it_interval.tv_nsec/1000);

#if 0 
  struct timespec ts;
  ts.tv_sec = 26;
  ts.tv_nsec = 1000000;
   // once interrupted by signal, left time will store in second parameter
  while(nanosleep(&ts, &ts)) { };

  memset(&its, 0x00, sizeof(its));
  memset(&oits, 0x00, sizeof(oits));
  retval = timer_settime(trt, 0, &its, &oits);
  if (retval < 0) {
    printf("settime: %d\n", errno);
    perror("timer_settime stop timer");
  }
  printf("timer_settime done\n");
  retval = timer_delete(trt);
  if (retval < 0) {
    perror("timer_delete");
  }
  printf("timer_delete done\n");
#endif

#if 1
  do {
    retval = timer_getoverrun(trt);
    if (0 < retval)
      printf("run over num: %d\n", retval);

    if (sigsum < 0) {
      // 4. stop timer
      memset(&its, 0x00, sizeof(its));
      memset(&oits, 0x00, sizeof(oits));
      retval = timer_settime(trt, 0, &its, &oits);
      if (retval < 0) {
        printf("settime: %d\n", errno);
        perror("timer_settime stop timer");
        //break;
      }
      printf("second settime: value: %ldus, interval: %ldus\n",
            oits.it_value.tv_sec*1000000 + oits.it_value.tv_nsec/1000,
            oits.it_interval.tv_sec*1000000 + oits.it_interval.tv_nsec/1000);

      // 5. delete timer
      retval = timer_delete(trt);
      if (retval < 0) {
        perror("timer_delete");
      }
      break;
    }
  } while(1);

  struct timespec ts;
  ts.tv_sec = 26;
  ts.tv_nsec = 1000000;
   // once interrupted by signal, left time will store in second parameter
  while(nanosleep(&ts, &ts)) { };
  
#endif
  return retval;

}


