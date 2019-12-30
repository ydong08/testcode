/*sigaction 使用*/
#include<stdio.h>
#include<signal.h>

void fa(int signo){
printf("捕获了信号%d\n",signo);
sleep(5);//每个信号在处理期间,默认屏蔽相同信号
printf("信号处理完毕\n");
}
int main(){
	struct sigaction action={};
	action.sa_handler=fa;//用第一个函数指针
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask,3);//处理信号时屏蔽信号3
	//action.sa_flags=SA_ONESHOT;//SA_RESETHAND
	//SA_ONESHOT执行一次处理函数恢复为默认处理
	action.sa_flags=SA_NOMASK;//处理期间,不屏蔽相同信号
	sigaction(SIGINT,&action,NULL);
	while(1);
}
