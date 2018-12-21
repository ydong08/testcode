#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int dup_test()
{
    char buf[10] = {0};
    const char* pfile = "dup.txt";
    memcpy(buf, "abcdefghijk", sizeof("abcdefghijk"));
    int fd = open(pfile, O_CREAT|O_WRONLY, 0664);
    printf("file fd: %d\n", fd);
    write(fd, buf, 10);

    int newfd = dup(fd);
    memcpy(buf, "0123456789", sizeof("0123456789"));
    printf("file fd: %d\n",newfd);
    write(newfd, buf, 10);

    newfd = dup2(newfd, newfd+2);
    memcpy(buf, "opqrstuvwxyz", sizeof("opqrstuvwxyz"));
    printf("file fd: %d\n",newfd);
    write(newfd, buf, 10);

    return 0;
}
