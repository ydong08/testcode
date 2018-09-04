#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define SIG_BUF_LEN        256 
void myaction(int signo, siginfo_t* psig, void* pdata) {
    if (SIGINT == signo) {
        printf("recv SIGINT signal\n");
        printf("sigval: %d\n", psig->si_value.sival_int);
        printf("*****************\n");
    }
}

int main() {
    char buf[SIG_BUF_LEN] = {0};
    struct sigaction sign;
    struct sigaction sigo;
    memset(&sign, 0x00, sizeof(sign));
    memset(&sigo, 0x00, sizeof(sigo));
    sign.sa_flags = SA_SIGINFO|SA_RESTART;
    sigemptyset(&sign.sa_mask);
    sign.sa_sigaction = myaction;
    int retval = sigaction(SIGINT, &sign, &sigo);
    if (retval < 0) {
        printf("sigaction: %d\n", errno);
        exit(1);
    }
    union sigval sigv;
    memset(&sigv, 0x00, sizeof(sigv));
    sigv.sival_int = 1234;
    retval = sigqueue(getpid(), SIGINT, sigv);
    if (retval < 0) {
        printf("sigqueue: %d\n", retval);
        exit(1);
    }

    memset(buf, 0x00, sizeof(buf));
    retval = read(0, buf, 100);
    if (retval < 0) {
        printf("read fail: %s\n", strerror(errno));
        perror("read");
        exit(1);
    }
    buf[sizeof(buf)-1] = '\0';
    printf("read %d bytes\n", retval);
    printf("read buf: %s\n", buf);
    return 0;
}

