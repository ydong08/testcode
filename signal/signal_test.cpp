#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <signal.h>

void handler(int a){
	
	printf("enter handle function\n");
	sleep(2);
	printf("leave handle function\n");
	return;
}

void sig_handler(int sigt, siginfo_t* sigf, void* p){
	if (sigt == SIGINT || sigt == SIGQUIT || sigt == SIGHUP)
		printf("To deal a No-real-sig: %d \n", sigt);
	else 
		printf("To deal a real-sig: %d \n", sigt);
}

int main(){
	struct sigaction atc;
	sigset_t  oldsig;
	sigemptyset(&atc.sa_mask);
	sigemptyset(&oldsig);

	sigaddset(&atc.sa_mask, SIGINT);
	sigaddset(&atc.sa_mask, SIGQUIT);
	sigaddset(&atc.sa_mask, SIGHUP);
	sigaddset(&atc.sa_mask, SIGRTMIN);
	sigaddset(&atc.sa_mask, SIGRTMIN + 1);

	sigprocmask(SIG_BLOCK, &atc.sa_mask, &oldsig);

	memset(&atc, 0, sizeof(struct sigaction));
	atc.sa_flags = SA_SIGINFO;
	atc.sa_sigaction = sig_handler;
	if (sigaction(SIGINT, &atc, NULL) < 0)
		printf("reg SIGINT failed\n");

	if (sigaction(SIGQUIT, &atc, NULL) < 0)
		printf("reg SIGQUIT failed\n");

	if (sigaction(SIGHUP, &atc, NULL) < 0)
		printf("reg SIGHUP failed\n");

	if (sigaction(SIGRTMIN, &atc, NULL) < 0)
		printf("reg SIGRTMIN failed\n");

	if (sigaction(SIGRTMIN+1, &atc, NULL) < 0)
		printf("reg SIGRTMIN+1 failed\n");

	printf("pid: %d\n", getpid());
	sleep(30);

	sigprocmask(SIG_SETMASK, &oldsig, NULL);

	return 0;
}