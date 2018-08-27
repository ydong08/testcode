#include "my_socket.h"
#define MY_IP "192.168.1.100"
#define MY_PORT 6666
#define SER_IP "192.168.1.100"
#define SER_PORT 8888
#define SIZE 192
#define MSG_SIZE (SIZE - 4)
typedef struct tag_mag 
{
    int msg_len ;
    char msg_buf[MSG_SIZE];//188
}MSG, *pMSG;
int main(int argc, char* argv[])
{
    int sfd ;
    my_socket(&sfd, MY_TCP, MY_IP, MY_PORT);
    my_connect(sfd, SER_IP, SER_PORT);
    MSG my_msg ;
    while(memset(&my_msg, 0, sizeof(MSG)), fgets(my_msg.msg_buf, MSG_SIZE, stdin)!= NULL)
    {
        my_msg.msg_len = strlen(my_msg.msg_buf);
        my_send(NULL, sfd, &my_msg, 4 + my_msg.msg_len );
        memset(&my_msg, 0, sizeof(MSG));
        my_recv(NULL, sfd, &my_msg, 4);
        my_recv(NULL, sfd, &my_msg.msg_buf, my_msg.msg_len);
        printf("recv from server : %s \n", my_msg.msg_buf);
    
    }
    close(sfd);

}