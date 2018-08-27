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
#define WORKER 4
#define LISTENNUM 10


int worker(int listenfd, int i){
		while(1){
			printf("I am worker %d, begin to accept connection.\n", i);
			struct sockaddr_in clientad;
			socklen_t size = sizeof(clientad);
			int newfd = accept(listenfd, (struct sockaddr*)&clientad, &size);
			if(newfd > 0){
				printf("worker %d accept a connection success.\t", i);
				printf("ip :%s\t",inet_ntoa(clientad.sin_addr));
				printf("port: %d \n",clientad.sin_port);
			}else{
				printf("worker %d accept a connection failed,error:%s", i, strerror(errno));
				//close(newfd);
			}
		}
		return 0;
}


int main(int argc, char* argv[]){
	/*
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd  < 0)
		perror("socket");
	struct sockaddr_in maddr;
	maddr.sin_family = PF_INET;
	maddr.sin_port = htons(PORT);
	inet_pton(PF_INET, IP, &maddr.sin_addr);

	if(bind(sockfd, (struct sockaddr*)&maddr, sizeof(maddr)) < 0)
		perror("bind");

	if(listen(sockfd, LISTENNUM) < 0)
		perror("listen");


	for(int i = 0; i < WORKER; ++i){
		 printf("Create worker %d\n", i+1);
		 pid_t pid = fork();
		 if(pid<0)
			 printf("fork error");
		 if(pid == 0)
			 worker(sockfd, i+1);
		 //else
			// exit(-1);
	}
	int stats;
	wait(&stats);//thread join
	*/
	pid_t pid = getpid();
	pid_t tid = syscall(SYS_gettid);

	pid_t grpid = getpgid(pid);
	pid_t pgrp = getpgrp();

	pid_t sid = getsid(pid);
	printf("pid: %d, tid: %d, grpid:%d, pgrp:%d, sid:%d\n", pid, tid, grpid, pgrp, sid); 

	return 0;
}


