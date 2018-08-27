#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define SERV_PORT 8080
#define LIST 20                //����������������
#define MAX_FD 10              //FD_SET֧������������


int main(int argc, char *argv[])
{
	int sockfd;
	int err;
	int i;
	int connfd;
	int fd_all[MAX_FD]; //��������������������select���ú��ж��ĸ��ɶ�
	
	//������������ԭ����select���ú󣬻ᷢ���仯���ٴε���selectǰ����Ҫ���¸�ֵ
	fd_set fd_read;    //FD_SET���ݱ���
    fd_set fd_select;  //����select

	struct timeval timeout;         //��ʱʱ�䱸��
	struct timeval timeout_select;  //����select
	
	struct sockaddr_in serv_addr;   //��������ַ
	struct sockaddr_in cli_addr;    //�ͻ��˵�ַ
	socklen_t serv_len;
	socklen_t cli_len;
	
	//��ʱʱ������
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	
	//����TCP�׽���
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("fail to socket");
		exit(1);
	}
	
	// ���ñ��ص�ַ
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;			// ipv4
	serv_addr.sin_port = htons(SERV_PORT);	// �˿ڣ� 8080
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // ip

    serv_len = sizeof(serv_addr);
	
	// ��
	err = bind(sockfd, (struct sockaddr *)&serv_addr, serv_len);
	if(err < 0)
	{
		perror("fail to bind");
		exit(1);
	}

	// ����
	err = listen(sockfd, LIST);
	if(err < 0)
	{
		perror("fail to listen");
		exit(1);
	}
	
	//��ʼ��fd_all����
	memset(fd_all, -1, sizeof(fd_all));

	fd_all[0] = sockfd;   //��һ��Ϊ�����׽���
	
	FD_ZERO(&fd_read);	// ���
	FD_SET(sockfd, &fd_read);  //�������׽��ּ���fd_read

	int maxfd = fd_all[0];  //����������׽���
	
	while(1){
	
		// ÿ�ζ���Ҫ���¸�ֵ��fd_select��timeout_selectÿ�ζ����
        fd_select = fd_read;
        timeout_select = timeout;
		
		// �������׽����Ƿ�ɶ���û�пɶ����˺���������
		// ֻҪ�пͻ����ӣ���Ͽ����ӣ�select()��������ִ��
		err = select(maxfd+1, &fd_select, NULL, NULL, NULL);
		//err = select(maxfd+1, &fd_select, NULL, NULL, (struct timeval *)&timeout_select);
		if(err < 0)
		{
				perror("fail to select");
				exit(1);
		}

		if(err == 0){
            printf("timeout\n");
		}
		
		// �������׽����Ƿ�ɶ�
		if( FD_ISSET(sockfd, &fd_select) ){//�ɶ���֤�����¿ͻ������ӷ�����
			
            cli_len = sizeof(cli_addr);
			
			// ȡ���Ѿ���ɵ�����
			connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
			if(connfd < 0)
			{
				perror("fail to accept");
				exit(1);
			}
			
			// ��ӡ�ͻ��˵� ip �Ͷ˿�
			char cli_ip[INET_ADDRSTRLEN] = {0};
			inet_ntop(AF_INET, &cli_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
			printf("----------------------------------------------\n");
			printf("client ip=%s,port=%d\n", cli_ip,ntohs(cli_addr.sin_port));
			
			// ���������׽��ּ��� fd_all �� fd_read
			for(i=0; i < MAX_FD; i++){
				if(fd_all[i] != -1){
					continue;
				}else{
					fd_all[i] = connfd;
					printf("client fd_all[%d] join\n", i);
					break;
				}
			}
			
			FD_SET(connfd, &fd_read);
			
			if(maxfd < connfd)
			{
				maxfd = connfd;  //����maxfd
			}
		
		}
		
		//��1��ʼ�鿴�����׽����Ƿ�ɶ�����Ϊ�����Ѿ������0��sockfd��
		for(i=1; i < maxfd; i++){
			if(FD_ISSET(fd_all[i], &fd_select)){
				printf("fd_all[%d] is ok\n", i);
				
				char buf[1024]={0};  //��д������
				int num = read(fd_all[i], buf, 1024);
				if(num > 0){

					//�յ� �ͻ������ݲ���ӡ
					printf("receive buf from client fd_all[%d] is: %s\n", i, buf);
					
					//�ظ��ͻ���
					num = write(fd_all[i], buf, num);
					if(num < 0){
						perror("fail to write ");
						exit(1);
					}else{
						//printf("send reply\n");
					}
					
				}
				else if(0 == num){ // �ͻ��˶Ͽ�ʱ
					
					//�ͻ����˳����ر��׽��֣����Ӽ����������
					printf("client:fd_all[%d] exit\n", i);
					FD_CLR(fd_all[i], &fd_read);
					close(fd_all[i]);
					fd_all[i] = -1;
					
					continue;
				}
				
			}else {
                //printf("no data\n");                  
            }
		}
	}
	
    return 0;
}