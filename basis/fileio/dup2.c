#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void main()
{
    int oldfd;
    int fd;
    int t;
    char *buf="This is a test!!!!\n";
    if((oldfd=open("mine.txt",O_RDWR|O_CREAT,0644))==-1)
    {
        printf("open error\n");
        exit(-1);
    }

    fd=dup2(oldfd,fileno(stdout));
    if(fd==-1)
    {
        printf("dup2 error\n");
        exit(-1);
    }
    printf("dup2的返回值：%d\n",fd);
    
    t=strlen(buf);
    //本应该写入到stdout的信息，但是标准输出已经重定向到目标文件中，故向标准输出写的数据将会写到目标文件中。
    if(write(fileno(stdout),buf,t)!=t)
    {
        printf("write error!\n");
        exit(-1);
    }
    close(fd);
    exit(0);
}
