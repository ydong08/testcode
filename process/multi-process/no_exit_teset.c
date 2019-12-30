#include  <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

void thread(void* p) {
    int loop = 100;
    do
    {
        printf("thread loop");
        sleep(1);
    } while(loop-- > 0);
}

int main()
{
    pid_t pid = fork();
    if (pid < 0){
        perror("fork");
        exit(-1);
    }

    if (0 == pid) {
        printf("child pid: %d\n", pid);
        
    } else {
        printf("parent pid: %d\n", pid);
        sleep(100);
        waitpid(-1, NULL, WNOHANG);
        printf("wait child\n");
        sleep(100);
    }
    
    return 0;
}
