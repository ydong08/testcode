#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>


using namespace std;

const int PATHLEN = 255;

void* getpwd(void* p)
{
    printf("enter    getpwd\n");
    char pathbuf[PATHLEN] = {0};
    if(-1 == readlink("/proc/self/exe", pathbuf, sizeof(pathbuf)))
    {
        printf("Error: %s\n", strerror(errno));
        perror("readlink: ");
        exit(-1);
    }

    printf("path: %s\n", pathbuf);
    return NULL;

}

void* createSub3Thread(void* p)
{
    printf("enter    sub3    thread\n");
    pthread_t pid;
    pthread_attr_t pstat;
    pthread_attr_init(&pstat);
    pthread_attr_setdetachstate(&pstat, PTHREAD_CREATE_JOINABLE);
    if(0 != pthread_create(&pid, NULL, getpwd, NULL))
    {
        perror("sub thread: ");
        exit(-1);
    }

    pthread_join(pid, NULL);
    return NULL;
}

void* createSub2Thread(void* p)
{
    printf("enter    sub2    thread\n");
    pthread_t pid;
    pthread_attr_t pstat;
    pthread_attr_init(&pstat);
    pthread_attr_setdetachstate(&pstat, PTHREAD_CREATE_JOINABLE);
    if(0 != pthread_create(&pid, NULL, createSub3Thread, NULL))
    {
        perror("sub thread: ");
        exit(-1);
    }

    pthread_join(pid, NULL);
    return NULL;
}

void* createSubThread(void* p)
{
    printf("enter    sub     thread\n");
    pthread_t pid;
    pthread_attr_t pstat;
    pthread_attr_init(&pstat);
    pthread_attr_setdetachstate(&pstat, PTHREAD_CREATE_JOINABLE);
    if(0 != pthread_create(&pid, NULL, createSub2Thread, NULL))
    {
        perror("sub thread: ");
        exit(-1);
    }

    pthread_join(pid, NULL);
    return NULL;
}

int main(int argc, char* argv[])
{

    pthread_t pid = 0;
    pthread_attr_t pattr;
    pthread_attr_init(&pattr);
    int pstat = PTHREAD_CREATE_JOINABLE;
    pthread_attr_setdetachstate(&pattr, pstat);

    if(0 != pthread_create(&pid, &pattr, createSubThread, NULL)){
        perror("pthread create");
        exit(-1);
    }

    printf("enter    main    thread\n");
    pthread_join(pid, NULL);
    printf("will exit main thread\n");

    return 0;
}


