#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

int arg_main()
{
	return printf("ARG_MAX: %ld\n", sysconf(_SC_ARG_MAX));
}

int curl_main()
{
	const char *cmd = "/userfs/bin/curl -v -sS -g -k -i --connect-timeout 5 --max-time 5 --speed-time 5 --speed-limit 5 http://192.168.80.101:8090/?info={\"name\":\"itibia\",\"average\":\"818.12\",\"ip\":\"172.168.41.109\",\"speed\":\"800.00\",\"maxSpeed\":\"900.00\",\"startTime\":\"20201102113030\",\"stopTime\":\"20201102113030\",\"downfilelarge\":\"123456\",\"percent\":\"80\",\"Source\":\"RMS\",\"WANInterface\":\"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2.WANPPPConnection.1.\",\"identify\":\"8e004f46-1dbb-44ea-a53b-126587ab9675\",\"type\":\"DownLink\"}";
	char  recvbuf[128] = {0};

	FILE *fp = popen(cmd, "r");
	if (!fp)
	{
		printf("popen error:%d:%s.\n", errno, strerror(errno));
		return -1;
	}

	fread(recvbuf, sizeof(recvbuf) - 1, 1, fp);
	printf("recvbuf:%s.\n", recvbuf);

	return 0;
}



int main(int argc, char* argv[])
{
	return arg_main();
//	return curl_main();
#if 0
	int  num = 100;
	int socketfd = 0;
	char *msg= "this is test content.\n";
	char recvbuf[256] = {0};

	socketfd = socket(AF_INET,SOCK_STREAM,0);
	if(socketfd == -1)
	{
		perror("socket error");
		exit(1);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	
	connect(socketfd,(struct sockaddr *)(&server_addr),sizeof(server_addr));
	do
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		write(socketfd, msg, strlen(msg));
		sleep(5);
		read(socketfd, recvbuf, sizeof(recvbuf));
		printf("recv: %s\n", recvbuf);

	} while(0 < num--);
	
	close(socketfd);
	printf("My Web Client is Ready!\n");
#endif
}
