#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/syscall.h>
#include<sys/types.h>

int pid_test(void)
{
    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid);

    pid_t grpid = getpgid(pid);
    pid_t pgrp = getpgrp();
    
    pid_t sid = getsid(pid);
    printf("pid: %d, tid: %d, grpid:%d, pgrp:%d, sid:%d\n", pid, tid, grpid, pgrp, sid); 

    return 0;
}