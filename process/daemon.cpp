/*
 * test daemon process
*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/stat.h>
#include<signal.h>
#include<fcntl.h>

#define BUFLEN 20

void daemon(){

	signal(SIGCHLD, SIG_IGN);

	pid_t pid = fork();
	if(pid < 0){
		perror("fork 1");
		exit(-1);
	}	
	else if(pid > 0)
		exit(0);

	setsid();

	pid = fork();
	if(pid < 0)
		exit(-1);
	else if(pid > 0)
		exit(0);

	struct  rlimit rl;
	if(getrlimit(RLIMIT_NOFILE, &rl)<0)
		exit(-1);
	if(rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for(int i = 0; i < rl.rlim_max; ++i)
		close(i);

	int fd = open("/dev/null", O_RDWR);
	if (0 < fd) {
		dup2(fd, 0);
		dup2(fd, 1);
		dup2(fd, 2);
	}

	chdir("/");

	umask(0);
	int ind = 0;
	char buf[BUFLEN] = {0};
	sprintf(buf, "%s_%d.%s", "daemon_", ind++, "log");
	FILE* fp = fopen(buf, "a+");

	while(ind > 0){
		sprintf(buf, "%d%c", ind++,'\n');
		fwrite(buf, sizeof(buf),1, fp);
	}

	return;
}

int main(){
	daemon();
	return 0;
}
