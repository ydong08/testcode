#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    struct tm tms;
    memset(&tms, 0x00, sizeof(tms));
    time_t tt = time(NULL);
    localtime_r(&tt, &tms);
    printf("DST: %d\n", tms.tm_isdst);
    printf("Date: %04d-%02d-%02d\n", tms.tm_year+1900, tms.tm_mon+1, tms.tm_mday);
    printf("Time: %02d-%02d-%02d\n", tms.tm_hour, tms.tm_min, tms.tm_sec);
    return 0;

}