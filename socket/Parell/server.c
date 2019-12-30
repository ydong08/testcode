/*************************************************************************
  > File Name: server.c
  > Author: KrisChou
  > Mail:zhoujx0219@163.com
  > Created Time: Fri 05 Sep 2014 03:31:12 PM CST
 ************************************************************************/
#include "my_socket.h"
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#define MY_IP "192.168.1.100"
#define MY_PORT 8888
#define SIZE 192
#define MSG_SIZE (SIZE - 4)
extern int errno ;
typedef struct tag_mag
{
    int  msg_len;//记录msg_buf的真实大小
    char msg_buf[MSG_SIZE];//msg_buf所占空间为188byte
}MSG, *pMSG;

void my_handle(int num)
{
    /*waitpid参数：
     * -1表示回收每一个儿子
     * NULL表示不关心子进程的exit的返回值
     * WNOHANG：wait no hang 非阻塞模式
     *waitpid返回值：
     * -1表示没有创建任何儿子
     *  0表示没有儿子退出
     *大于0表示有儿子退出
     * */
    while(waitpid(-1, NULL, WNOHANG ) > 0) ;
}

int main(int argc, char* argv[])
{
    int fd_listen , fd_client ;
    signal(SIGCHLD, my_handle);
    my_socket(&fd_listen, MY_TCP, MY_IP ,MY_PORT);
    my_listen(fd_listen, 10);

    while( fd_client = accept(fd_listen, NULL, NULL))
    {
        /* 只要不是程序默认忽略的信号，accept都能收到，并返回-1 */
        if(fd_client == -1)
        {
            if(errno == EINTR)
            {
                continue ;
            }else 
            {
                break ; //break退出后，父亲就退了。下来儿子会由init接管。
            }
        }else 
        {
            if(fork() == 0) //fork儿子用于与客户端通信
            {
                MSG recv_msg ;
                int recvn;
                    while(1 )
                    {
                        memset(&recv_msg, 0, sizeof(MSG));
                        /*在my_socket.c中，my_recv接收的长度 与 my_send 发送的长度必须是精确值 
                         * my_recv中填的长度小于等于实际要收的，是可以的，大于的话就永远退不出循环了*/
                        my_recv(&recvn, fd_client, &recv_msg, 4);
                        if(recvn == 0) //当对面客户端退出（关闭socket）,系统调用recv的返回值为0
                        {
                            break ;
                        }else
                        {
                            my_recv(NULL,fd_client, &recv_msg.msg_buf, recv_msg.msg_len);
                            my_send(NULL, fd_client, &recv_msg, 4 + recv_msg.msg_len);

                        }
                    }
                close(fd_client);
                exit(0);    
            }
            close(fd_client);
        }
    }
    return 0 ;
}