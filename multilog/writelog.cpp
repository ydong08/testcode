#include "writelog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_FILE        "./log"
#define LOWRITELOG      "abcdefghijklmnopqrstuvwxyz\n"
void writelog() {
    FILE* pf = fopen(LOG_FILE, "a");
    if (NULL == pf)
    {
        perror("[so]fopen");
    }
    printf("[so]open file fd: %d\n", fileno(pf));
    fwrite(LOWRITELOG, 1, strlen(LOWRITELOG), pf);
    printf("[so]whence: %ld\n", ftell(pf));
    fflush(pf);
    fclose(pf);
    return ;
}

