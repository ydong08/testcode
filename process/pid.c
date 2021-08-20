#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/wait.h>
#include<sys/syscall.h>

#define IP	"127.0.0.1"
#define  PORT 8888

int main(int argc, char* argv[])
{
	char buf[sizoef(struct sin_addr)] = {0};
	inet_pton(AF_INET, IP, buf);
	printf("IP:%d\n", buf);
	pid_t pid = getpid();
	pid_t tid = syscall(SYS_gettid);

	pid_t grpid = getpgid(pid);
	pid_t pgrp = getpgrp();

	pid_t sid = getsid(pid);
	printf("pid: %d, tid: %d, grpid:%d, pgrp:%d, sid:%d\n", pid, tid, grpid, pgrp, sid); 

	return 0;
}


