#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

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
    struct timeval iternalval = {8, 10};
    struct timeval leftval = {10, 10};
    struct timeval iternalval2 = {24, 30};
    struct timeval leftval2 = {20, 10};
    newlit.it_interval = iternalval;
    newlit.it_value = leftval;
    newrit.it_interval = iternalval2;
    newrit.it_value = leftval2;
    // ITIMER_VIRTUAL ITIMER_REAL
    retval = setitimer(ITIMER_VIRTUAL, &newlit, &oldit);
    if (retval < 0) {
        perror("setitimer");
        exit(1);
    }
    retval = setitimer(ITIMER_REAL, &newlit, &oldit);  
    if (retval < 0) {
        perror("setitimer");
        exit(1);
    }
    retval = setitimer(ITIMER_REAL, &newrit, &oldit);  // overwrite
    if (retval < 0) {
        perror("setitimer");
        exit(1);
    }

    do 
    {
        memset(&oldit, 0x00, sizeof(oldit));
        retval = getitimer(ITIMER_REAL, &oldit);
        if (retval < 0) {
            perror("getitimer");
            exit(1);
        }
        printf("interval: %08ld, leftval: %08ld\n", 
                    oldit.it_interval.tv_sec*1000000+oldit.it_interval.tv_usec,
                    oldit.it_value.tv_sec*1000000+oldit.it_value.tv_usec);
        printf("interval.sec: %d, interval.usec: %d\n", oldit.it_interval.tv_sec, oldit.it_interval.tv_usec);
        printf("leftval.sec: %d, leftval.usec: %d\n", oldit.it_value.tv_sec, oldit.it_value.tv_usec);
        printf("interval: %08ld, leftval: %08ld\n", oldit.it_interval.tv_sec, oldit.it_value.tv_sec);

        memset(&oldit, 0x00, sizeof(oldit));
        retval = getitimer(ITIMER_VIRTUAL, &oldit);
        if (retval < 0) {
            perror("getitimer");
            exit(1);
        }
        printf("----------------------------------\n");
        printf("interval: %08ld, leftval: %08ld\n", 
                    oldit.it_interval.tv_sec*1000000+oldit.it_interval.tv_usec,
                    oldit.it_value.tv_sec*1000000+oldit.it_value.tv_usec);
        printf("interval.sec: %d, interval.usec: %d\n", oldit.it_interval.tv_sec, oldit.it_interval.tv_usec);
        printf("leftval.sec: %d, leftval.usec: %d\n", oldit.it_value.tv_sec, oldit.it_value.tv_usec);
        float interval = 0;
        float value = 0;
        // interval = oldit.it_interval.tv_sec+oldit.it_interval.tv_usec/1000000;
        // value = oldit.it_value.tv_sec+oldit.it_value.tv_usec/1000000;
        printf("interval: %f, leftval: %f\n", 
                    interval,
                    value);
        printf("interval: %f, leftval: %f\n", 
                    oldit.it_interval.tv_sec+oldit.it_interval.tv_usec/1000000,
                    oldit.it_value.tv_sec+oldit.it_value.tv_usec/1000000);
        printf("----------------------------------\n");
        long sec = 24;
        float fsec = 24.6;
        printf("sec:%%d = %d\n", sec);
        printf("fsec:%%d = %d\n", fsec);
        printf("sec:%%f = %f\n", sec+(sec/1000000));
        printf("fsec:%%f = %f\n", fsec);
        printf("fsec:%%f = %f\n", floor(fsec));
        printf("fsec:%%f = %f\n", ceil(fsec));
    } while(0);

    return 0;
}



