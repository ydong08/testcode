#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void printf_sig(sigset_t* psig);

int main() {
    //sigaction 
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGABRT);
    sigaddset(&sigset, SIGQUIT);
    printf_sig(&sigset);
    sigaddset(&sigset, SIGALRM);
    sigaddset(&sigset, SIGRTMIN);
    sigaddset(&sigset, SIGUSR1);
    printf_sig(&sigset);
    sigdelset(&sigset, SIGUSR1);
    sigfillset(&sigset);
    printf_sig(&sigset);
    sigemptyset(&sigset);
    printf_sig(&sigset);

    return 0;
}

void printf_sig(sigset_t* psig) {
    for (int i = 1; i < NSIG; ++i) {
        if (sigismember(psig, i))
            printf("1,");
        else
            printf("0,");
    }
    printf("\n");
    return ;
}

