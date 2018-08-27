
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
    printf("enter fork\n");
    fflush(NULL);
    pid_t pid = fork();
    if (0 > pid ){
        perror("fork");
        exit(1);
    }

    if (0 == pid ){
        printf("enter child process\n");
        loop_print();
        exit(0);
    } else {
        /*
        // execl() 
        execl("/bin/ls", "ls", "-l", NULL);
        */


        perror("execl");
       // wait(NULL);
       exit(0);
    }
     printf("will exit parent process\n");
}