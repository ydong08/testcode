#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//#define POSIX_SIGINFO

void myhandler(int signo) {
    if (SIGINT == signo) {
        printf("recv SIGINT signal\n");
        printf("++++++++++++++++\n");
    }
}

void myaction(int signo, siginfo_t* psig, void* pdata) {
    if (SIGINT == signo) {
        printf("recv SIGINT signal\n");
        printf("sigval: %d\n", psig->si_value.sival_int);
        printf("****************\n");
    }
}

int main() {
    struct sigaction sign;
    struct sigaction sigo;
    memset(&sign, 0x00, sizeof(sign));
    memset(&sigo, 0x00, sizeof(sigo));
    // SA_RESTART 会让中断的调用在信号处理完成后自动执行
#ifdef POSIX_SIGINFO
    sign.sa_flags = SA_SIGINFO|SA_RESTART;
    sign.sa_sigaction = myaction;
#else
    // sign.sa_flags = 0;
    sign.sa_flags = SA_SIGINFO|SA_RESTART;
    sign.sa_handler = myhandler;
#endif
    sigemptyset(&sign.sa_mask);
    sigaddset(&sign.sa_mask, SIGINT);
    int retval = sigaction(SIGINT, &sign, &sigo);
    if (retval < 0) {
        printf("sigaction: %d\n", errno);
        exit(1);
    }
    union sigval sigv;
    memset(&sigv, 0x00, sizeof(sigv));
    sigv.sival_int = 1000;
    retval = sigqueue(getpid(), SIGINT, sigv);
    if (retval < 0) {
        printf("sigqueue: %d\n", errno);
        exit(1);
    }
    return 0;
}




