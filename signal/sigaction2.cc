#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

void fa(int signo,siginfo_t* info,void*p){
	printf("进程%d发来信号%d,附带数据%d\n",info->si_pid,signo,info->si_value);
}
int main(){
	struct sigaction action={};
	action.sa_flags=SA_SIGINFO;
	action.sa_sigaction=fa;
	sigaction(50,&action,NULL);//50为可靠信号,用不可靠信号时,只发送一次信号
	pid_t pid=fork();
	if(pid==0){
	int i;
	for(i=0;i<100;i++){
	union sigval v;
	v.sival_int=i;
	sigqueue(getppid(),50,v);
}
	sleep(1);
	exit(0);
}
	while(1);
}
