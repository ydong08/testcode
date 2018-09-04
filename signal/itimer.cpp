#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

void myhandler(int signo) {
    switch(signo) {
        case SIGALRM:
            printf("recv SIGALRM signal\n");
            break;
        case SIGVTALRM:
            printf("recv SIGVTALRM signal\n");
            break;
        default:
            printf("recv unknown signal\n");
            break;
    }
}


int main() {
    struct sigaction sign;
    struct sigaction sigo;
    memset(&sign, 0x00, sizeof(sign));
    memset(&sigo, 0x00, sizeof(sigo));
    sign.sa_flags = 0;
    sigemptyset(&sign.sa_mask);
    sigaddset(&sign.sa_mask, SIGALRM);
    sigaddset(&sign.sa_mask, SIGVTALRM);
    sign.sa_handler = myhandler;
    int retval = sigaction(SIGALRM, &sign, &sigo);
    if (retval < 0) {
        perror("sigaction");
        exit(1);
    }

    retval = sigaction(SIGVTALRM, &sign, &sigo);
    if (retval < 0) {
        perror("sigaction");
        exit(1);
    }

    struct itimerval newlit;
    struct itimerval newrit;
    struct itimerval oldit;
    memset(&newlit, 0x00, sizeof(newlit));
    memset(&newrit, 0x00, sizeof(newrit));
    memset(&oldit, 0x00, sizeof(oldit));
    struct timeval iternalval = {5, 10};
    struct timeval leftval = {10, 10};
    newlit.it_interval = iternalval;
    newlit.it_value = leftval;
    newrit.it_interval = iternalval;
    newrit.it_value = leftval;
    retval = setitimer(ITIMER_VIRTUAL, &newlit, &oldit);
    if (retval < 0) {
        perror("setitimer");
        exit(1);
    }
    usleep(1000000);
    retval = setitimer(ITIMER_REAL, &newrit, &oldit);
    if (retval < 0) {
        perror("setitimer");
        exit(1);
    }
    usleep(2000000);
    printf("interval: %08lf, leftval: %08lf\n", 
            oldit.it_interval.tv_sec+(oldit.it_interval.tv_usec/1000000),
            oldit.it_value.tv_sec+(oldit.it_value.tv_usec/1000000));
    do 
    {
        retval = getitimer(ITIMER_REAL, &newrit);
        if (retval < 0) {
            perror("getitimer");
            exit(1);
        }
        printf("interval: %08lf, leftval: %08lf\n", 
                    newrit.it_interval.tv_sec+(newrit.it_interval.tv_usec/1000000),
                    newrit.it_value.tv_sec+(newrit.it_value.tv_usec/1000000));

        retval = getitimer(ITIMER_VIRTUAL, &newlit);
        if (retval < 0) {
            perror("getitimer");
            exit(1);
        }
        printf("interval: %08lf, leftval: %08lf\n", 
                    newlit.it_interval.tv_sec+(newlit.it_interval.tv_usec/1000000),
                    newlit.it_value.tv_sec+(newlit.it_value.tv_usec/1000000));
        usleep(2000000);
    } while(1);

    return 0;
}

