#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void my_signal(int signo) {
    if (SIGABRT == signo) {
        printf("recv a ABORT signal\n");
    }
}

int main() {
    signal(SIGABRT, my_signal);
    int procstatus = 0;
    int retval = 0;
    char* const parabuf[] = {"ls", "-l", "-a", "-r", "-t", NULL};
    char* const envvar[] = {"PATH=/bin:/usr/bin", "TERM=console", NULL};
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(-1);
    }

    if (0 == pid) { // child
        printf("gen child process\n");
        //retval = execl("/usr/bin/ls", "ls", "-l", "-t", NULL);
        //retval = execle("/usr/bin/ls", "ls", "-l", "-t", NULL, envvar);
        //retval = execlp("ls", "ls", "-l", "-t", NULL);
        //retval = execv("/usr/bin/ls", parabuf);
        //retval = execvp("ls", parabuf);
        //retval = execve("/usr/bin/ls", parabuf, envvar);
        //retval = execve("/usr/bin/ls", parabuf, envvar);
        if (retval < 0) {
            printf("%d process exec fail\n", getpid());
            perror("excel");
            abort();
        }
        exit(3);
    } else {  // parent
        wait(&procstatus);
        printf("release %d child process, info:0x%08X\n", pid, procstatus);
        if (WIFEXITED(procstatus)) {
            printf("child process exit correctly: 0x%08x\n", WEXITSTATUS(procstatus));
        } else if (WIFSIGNALED(procstatus)) {
            printf("child process exit signal: 0x%08X\n", WTERMSIG(procstatus));
        } else if (WIFSTOPPED(procstatus)) {
            printf("child process stop: 0x%08X\n", WSTOPSIG(procstatus));
        }
        printf("%d parent process is run\n", getpid());
        printf("Am parent process\n");
        exit(0);
    }
}