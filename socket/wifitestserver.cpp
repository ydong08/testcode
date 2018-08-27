#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>

#define SOCK_NUM                        6
#define SND_BUF_LEN                 32
#define RCV_BUF_LEN                 32
#define BASE_PORT                       5020
#define LOCAL_IP                            "192.168.31.212"

#define SND_MSG     "abcdefghijklmnopqrst"

void* ProcessData(void* p)
{
    //pthread_detach(pthread_self());
    int nRet = -1;
    char szSndBuf[SND_BUF_LEN] = {0};
    char szRcvBuf[RCV_BUF_LEN] = {0};
    snprintf(szSndBuf, sizeof(szSndBuf), "%s", SND_MSG);
    int nfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > nfd)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in staddr;
    memset(&staddr, 0x00, sizeof(staddr));
    staddr.sin_family = AF_INET;
    staddr.sin_port = htons(*((unsigned short*)p));
    inet_pton(AF_INET, LOCAL_IP, &staddr.sin_addr);

    nRet = bind(nfd, (struct sockaddr*)&staddr, sizeof(staddr));
    if (0 > nRet)
    {
         perror("bind");
         exit(1);
    }

    do
    {
        nRet = send(nfd, szSndBuf, strlen(szSndBuf), 0);
        if (0 < nRet)
        {
             printf("Send: %04d - %s", nRet, szSndBuf);
        }

        memset(szRcvBuf, 0x00, sizeof(szRcvBuf));
        nRet = recv(nfd, szRcvBuf, sizeof(szRcvBuf), 0);
        if (0 < nRet)
        {
                printf("Recv: %04d - %s", nRet, szRcvBuf);
        }
       
    } while(1);
    
}


int main(int argc, char* argv[])
{
    int nRet = -1;
    int nBasePort = BASE_PORT;
    pthread_t nThreadIDS[SOCK_NUM] = {};
    int nThreadNum = 2;
    // if (argc > 1)
    // {
    //     nThreadNum  = strtol(argv[1], NULL, 10);   
    //     if (SOCK_NUM < nThreadNum)
    //         exit(1);
    // }

    int i = 0;
    for (; i < nThreadNum; ++i)
    {
        nRet = pthread_create(&nThreadIDS[i], NULL, ProcessData, &nBasePort);
        if (0 > nRet)
        {
           printf("%d pthread_create fail\n", i);
        }
        ++nBasePort;
    }

    i = 0;
    for (; i < nThreadNum; ++i)
    {
        pthread_join(nThreadIDS[i], NULL);
    }

    return 0;

}

