#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define LOOP 20
void loop_print(void)
{
    int nloop = 0;
    while(++nloop < LOOP){
        printf("loop times %d\n", nloop);
        sleep(1);
    }
}

int main()
{
	char *envs[] = {"exec=ok", "process=main", NULL};
	char *param[] = {"date", "+%T", NULL};
    printf("enter fork\n");
    fflush(NULL);
	int status;
    pid_t pid = fork();
    if (pid < 0){
        perror("fork");
        exit(1);
    }

    if (0 == pid )
	{
        printf("enter child process\n");
      
		//execl("/bin/date", "date", "+%T", NULL);
		//execlp("date", "date", "+%T", NULL);
		//execle("./echoenv", "echoenv", NULL, envs);

		execv("/bin/date", param);
		//execvp("date", param);
    	printf("exec error\n");
	}
	else
	{
		waitpid(pid, &status, 0);
		printf("exit parent process\n");		
	}
 	
 	return 0;
}
