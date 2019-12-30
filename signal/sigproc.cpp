#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sighandler(int signo) {

    if (SIGINT == signo) {
        printf("recv SIGINT signal\n");
        printf("*******************\n");
    } else if (SIGQUIT == signo) {
        printf("recv SIGQUIT signal\n");
        printf("++++++++++++++++++\n");
        sigset_t uset;
        sigemptyset(&uset);
        sigaddset(&uset, SIGINT);
        sigprocmask(SIG_UNBLOCK, &uset, NULL);
    } else if (SIGTERM == signo) {
        printf("recv SIGTERM signal\n");
        printf("------------------\n");
    } else if (SIGFPE == signo) {
        printf("recv SIGFPE signal\n");
        printf("$$$$$$$$$$$$$$$$$$\n");
    }

}

void print_sig(sigset_t* psig) {
    for (int i = 1; i < NSIG; ++i) {
        if (sigismember(psig, i))
            printf("1,");
        else
            printf("0,");
    }
    printf("\n");
}

int main() {
    sigset_t bset;
    sigset_t pset;
    sigset_t tset;
    sigemptyset(&bset);
    sigemptyset(&pset);
    sigemptyset(&tset);
    sigaddset(&bset, SIGINT);
    
    sigaddset(&tset, SIGTERM);
    sigaddset(&tset, SIGINT);
    sigaddset(&tset, SIGQUIT);
    if (SIG_ERR == signal(SIGINT, sighandler))
        printf("install SIGINT fail\n");
    if (SIG_ERR == signal(SIGQUIT, sighandler))
        printf("install SIGQUIT fail\n");
    if (SIG_ERR == signal(SIGFPE, sighandler))
        printf("install SIGFPE fail\n");
    sigprocmask(SIG_BLOCK, &bset, NULL);

    while(1) {
        sigpending(&pset);
        print_sig(&pset);
        usleep(1000000);
        sigsuspend(&tset);
        printf("exec sigsuspend done\n");
    }
    return 0;
}

// 