#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int loop = 100;
    const char* pctext = "abcdefghijkl\n";
    const char* pfile = "abc.txt";
   
    int cfd = open(pfile, O_CREAT|O_RDWR, 0664);
    printf("file fd2: %d\n", cfd);
    if (0 < cfd) {
        do {
            write(cfd, pctext, strlen(pctext)+1) ;
            usleep(1000*1000);
        } while (loop-- > 0);
    }
    exit(0);
}

