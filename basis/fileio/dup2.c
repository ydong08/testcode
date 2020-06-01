#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void main()
{
    int oldfd,oldfd2;
    int fd;
    int t;
    char *buf="This is a test!!!!\n";
    if((oldfd=open("mine.txt",O_RDWR|O_CREAT,0644))==-1)
    {
        printf("open error\n");
        exit(-1);
    }

    if((oldfd2=open("yours.txt",O_RDWR|O_CREAT,0644))==-1)
    {
        printf("open error\n");
        exit(-1);
    }

    fd=dup2(oldfd,oldfd2);
    if(fd==-1)
    {
        printf("dup2 error\n");
        exit(-1);
    }
    printf("dup2的返回值：%d\n",fd);
    
    t=strlen(buf);
	usleep(100000000);
	fprintf(stderr, "begin to close stdout\n");
	close(fd);
	fprintf(stderr, "end to close stdout\n");
	usleep(100000000);
#if 0
    //本应该写入到stdout的信息，但是标准输出已经重定向到目标文件中，故向标准输出写的数据将会写到目标文件中。
    if(write(fileno(stdout),buf,t)!=t)
    {
        printf("write error!\n");
        exit(-1);
    }
#endif
    close(fd);
	close(oldfd);
    exit(0);
}
