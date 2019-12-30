

#include "writelog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LOG_FILE        "./log"
#define HIWRITELOG        "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"

int main() {
    FILE* pf = fopen(LOG_FILE, "a");
    if (NULL == pf)
    {
        perror("[main]fopen");
    }
    printf("[main]open file fd: %d\n", fileno(pf));
    fwrite(HIWRITELOG, 1, strlen(HIWRITELOG), pf);
    printf("[main]whence: %ld\n", ftell(pf));
    sleep(2);
    writelog();
    sleep(2);
    fwrite(HIWRITELOG, 1, strlen(HIWRITELOG), pf);
    printf("[main]whence: %ld\n", ftell(pf));
    fflush(pf);
    fclose(pf);
    return 0;
}


