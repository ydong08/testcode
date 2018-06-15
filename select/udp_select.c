#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc,char *argv[])
{
	int udpfd = 0;
	struct sockaddr_in saddr;
	struct sockaddr_in caddr;

	bzero(&saddr,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port   = htons(8000);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bzero(&caddr,sizeof(caddr));
	caddr.sin_family  = AF_INET;
	caddr.sin_port    = htons(8000);
	
	//�����׽���
	if( (udpfd = socket(AF_INET,SOCK_DGRAM, 0)) < 0)
	{
		perror("socket error");
		exit(-1);
	}
	
	//�׽��ֶ˿ڰ���
	if(bind(udpfd, (struct sockaddr*)&saddr, sizeof(saddr)) != 0)
	{
		perror("bind error");
		close(udpfd);		
		exit(-1);
	}

	printf("input: \"sayto 192.168.220.X\" to sendmsg to somebody\033[32m\n");	
	while(1)
	{	
		char buf[100]="";	
		fd_set rset;	//�����ļ��������ľۺϱ���	
		FD_ZERO(&rset); //�ļ��������ۺϱ�����0
		FD_SET(0, &rset);//����׼������ӵ��ļ��������ۺϱ�����
		FD_SET(udpfd, &rset);//��udpfd��ӵ��ļ��������ۺϱ�����		
		write(1,"UdpQQ:",6);
		
		if(select(udpfd + 1, &rset, NULL, NULL, NULL) > 0)
		{
			if(FD_ISSET(0, &rset))//����0�Ƿ�ɶ�д
			{				
				fgets(buf, sizeof(buf), stdin);
				buf[strlen(buf) - 1] = '\0';
				if(strncmp(buf, "sayto", 5) == 0)
				{
					char ipbuf[16] = "";
					inet_pton(AF_INET, buf+6, &caddr.sin_addr);//��addr�׽��ֵ�ַ�ٸ�ֵ.
					printf("\rsay to %s\n",inet_ntop(AF_INET,&caddr.sin_addr,ipbuf,sizeof(ipbuf)));
					continue;
				}
				else if(strcmp(buf, "exit")==0)
				{
					close(udpfd);
					exit(0);
				}
				sendto(udpfd, buf, strlen(buf),0,(struct sockaddr*)&caddr, sizeof(caddr));
			}
			if(FD_ISSET(udpfd, &rset))//����udpfd�Ƿ�ɶ�д
			{
				struct sockaddr_in addr;
				char ipbuf[INET_ADDRSTRLEN] = "";
				socklen_t addrlen = sizeof(addr);
				
				bzero(&addr,sizeof(addr));
				
				recvfrom(udpfd, buf, 100, 0, (struct sockaddr*)&addr, &addrlen);
				printf("\r\033[31m[%s]:\033[32m%s\n",inet_ntop(AF_INET,&addr.sin_addr,ipbuf,sizeof(ipbuf)),buf);
			}
		}
	}
	
	return 0;
}


