/*

API test：

	getnameinfo
	getaddrinfo

	gethostbyaddr
	getservbyname
	getservbyport

	getpeername
	getsockname

*/


#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>


void* sock_thread(void* p)
{
	struct addrinfo *prp = NULL;
	int sfd = -1;
	int bytes = -1;
	int res = -1;
	socklen_t socklen = 0;
	struct sockaddr saddr;
	struct timespec mts,nts;
	char hostbuf[NI_MAXHOST] = {0};
	char servbuf[NI_MAXSERV] = {0};
	char buf[256] = {0};

	if (!p)
		return NULL;

	prp = p;
	socklen = sizeof(saddr);
	mts.tv_sec = 2;
	mts.tv_nsec = 100000000;

	pthread_deatch(pthread_self());
	sfd = socket(prp->ai_family, prp->ai_socktype, prp->ai_protocol);
	if (sfd < 0)
	{
		printf("socket fail:%d\n", errno);
		continue;
	}

	res = bind(sfd, (struct sockaddr*)prp->ai_addr, sizeof(struct sockaddr));
	if (res < 0)
	{
		print("bind fail:%d\n", errno);
	}

	while (1)
	{
		bytes = recvfrom(sfd, buf, sizeof(buf), 0, &saddr, &socklen);
		if (bytes < 0)
		{
			continue;
		}

		res = getnameinfo((struct sockaddr*)&saddr, socklen, hostbuf, sizeof(hostbuf), servbuf, NI_MAXSERV, NI_NUMBERICSERV);
		if (res)
		{
			printf("getnameinfo fail:%d\n", res);
			continue;
		}

		printf("recvfrom host:%s service:%s\n", hostbuf, servbuf);
		nanosleep(&mts, &nts);
	}

	pthread_exit(NULL);
}

int main()
{
	int res = -1;
	const char *node = "debian";
	const char *service = "sshd";

	struct addrinfo ai;
	struct addrinfo *pai = NULL, *prp;

	memset(&ai, 0, sizeof(ai));
	ai.ai_family = AF_UNSPEC;
	ai.ai_socktype = SOCK_DGRAM;
	ai.ai_protocol = 0;
	ai.ai_flags = AI_PASSIVE;

	res = getaddrinfo(node, service, &ai, &pai);
	if (res)
	{
		printf("getaddrinfo fail:%s\n", gai_strerror());
		return -1;
	}

	for (prp = pai; prp; prp=prp->ai_next)
	{
		printf("addrinfo:\n");
		printf("\tai_flags:%-#X\n", prp->ai_flags);
		printf("\tai_family:%-d\n", prp->ai_family);
		printf("\tai_socktype:%-d\n", prp->ai_socktype);
		printf("\tai_protocol:%-d\n", prp->ai_protocol);
		printf("\tai_addrlen:%-d\n", prp->ai_addrlen);
		printf("\tai_ai_canonname:%-s\n", prp->ai_canonname);
		printf("\taddrinfo sockaddr:\n");
		printf("\t\tai_addr.sin_family:%-d\n", prp->ai_addr.sin_family);
		printf("\t\tai_addr.sin_addr:%-s\n", inet_ntoa(prp->ai_addr.sin_addr));

		pthread_t tid;
		pthread_create(&tid, NULL, sock_thread, prp);
	}

	
	return 0;
}
