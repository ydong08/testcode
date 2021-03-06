#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main() {
    struct tm tms;
    memset(&tms, 0x00, sizeof(tms));
    struct timeval tv;
    memset(&tv, 0x00, sizeof(tv));
    struct timespec ts;
    memset(&ts, 0x00, sizeof(ts));
    const int TIME_STRING_LEN = 32;
    char tbuf[TIME_STRING_LEN] = {0};

    /*######### time_t -> time string ###########*/
    time_t tt = time(NULL);  // ms
    printf("time: %ld\n", tt);
    // 1. time_t -> tm
    gettimeofday(&tv, NULL);
    printf("time: %ld\n", tv.tv_sec*1000 + tv.tv_usec*1000);  // ms

    clock_gettime(CLOCK_REALTIME, &ts);
    printf("clocktime: %ld\n", ts.tv_sec*1000 + ts.tv_nsec*1000000);  // ms
    // gmtime(&tt, &tms);  // no timezone
    localtime_r(&tt, &tms); // + timezone
    printf("UTC: %ld\n", (long)tt);
    printf("DST: %d\n", tms.tm_isdst);
    printf("Date: %04d-%02d-%02d\n", tms.tm_year+1900, tms.tm_mon+1, tms.tm_mday);
    printf("Time: %02d-%02d-%02d\n", tms.tm_hour, tms.tm_min, tms.tm_sec);

    // 1. asctime: tm -> fix time string
    memset(tbuf, 0x00, sizeof(tbuf));
    printf("asctime: %s\n", asctime_r(&tms, tbuf));

    // 2. strftime: tm -> fix time string
    memset(tbuf, 0x00, sizeof(tbuf));
    strftime(tbuf, sizeof(tbuf), "%c", &tms);
    printf("strftime: %s\n", tbuf);
	strftime(tbuf, sizeof(tbuf), "%I:%M %p", &tms);
    printf("strftime: %s\n", tbuf);

    // time_t -> fix time string
    printf("ctime: %s\n", ctime_r(&tt, tbuf));

    /*######### time string -> time_t ###########*/
    // strptime: fix time string -> tm 
    strptime(tbuf, "%c", &tms);
    printf("[strptime]Date: %04d-%02d-%02d\n", tms.tm_year+1900, tms.tm_mon+1, tms.tm_mday);
    printf("[strptime]Time: %02d-%02d-%02d\n", tms.tm_hour, tms.tm_min, tms.tm_sec);

    // mktime: tm -> time_t
    tt = mktime(&tms);
    printf("[mktime]time: %ld\n", tt);

    return 0;

}
