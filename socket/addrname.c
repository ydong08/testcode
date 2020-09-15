
/* API test：

	getnameinfo / getaddrinfo

	gethostbyaddr / getservbyname / getservbyport

	getpeername / getsockname
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <netdb.h>
#include <syscall.h>
#include <unistd.h>

#define ADDR_INFO_CLIENT
#define ADDR_INFO_SERVER

if defined(ADDR_INFO_SERVER)
void* sock_thread(void* p)
{
	printf("create thread %d ok\n", syscall(SYS_gettid));
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
		goto EXIT;

	prp = p;
	socklen = sizeof(saddr);
	mts.tv_sec = 2;
	mts.tv_nsec = 100000000;

	//pthread_detach(pthread_self());
	sfd = socket(prp->ai_family, prp->ai_socktype, prp->ai_protocol);
	if (sfd < 0)
	{
		printf("socket fail:%d\n", errno);
		goto EXIT;
	}

	res = bind(sfd, (struct sockaddr*)prp->ai_addr, sizeof(struct sockaddr));
	if (res < 0)
	{
		printf("bind fail:%d\n", errno);
		goto EXIT;
	}

	while (1)
	{
		nanosleep(&mts, &nts);
		bytes = recvfrom(sfd, buf, sizeof(buf), 0, &saddr, &socklen);
		if (bytes < 0)
		{
			printf("recvfrom fail:%d\n", errno);
			continue;
		}

		res = getnameinfo((struct sockaddr*)&saddr, socklen, hostbuf, sizeof(hostbuf), servbuf, NI_MAXSERV, NI_NUMERICSERV);
		if (res)
		{
			printf("getnameinfo fail:%d\n", res);
			continue;
		}

		printf("recvfrom host:%s service:%s\n", hostbuf, servbuf);
	}

EXIT:
	pthread_exit(NULL);
}

/* gethostbyname, gethostbyaddr already obsolete */
int main()
{
	int res = -1;
	int i   = 0;
	const char *node = "debian";
	const char *service = "recvst 28011/tcp";
	pthread_t tid[10];

	struct addrinfo ai;
	struct addrinfo *pai = NULL, *prp;

	memset(&ai, 0, sizeof(ai));
	ai.ai_family = AF_UNSPEC;
	ai.ai_socktype = SOCK_STREAM;
	ai.ai_protocol = 0;
	ai.ai_flags = AI_PASSIVE;

	memset(tid, 0, sizeof(tid));
	res = getaddrinfo(node, /*service*/NULL, &ai, &pai);
	if (res)
	{
		printf("getaddrinfo fail:%s\n", gai_strerror(res));
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
		printf("\t\tai_addr.sin_family:%-d\n", ((struct sockaddr_in*)(prp->ai_addr))->sin_family);
		printf("\t\tai_addr.sin_port:%-d\n", htons(((struct sockaddr_in*)(prp->ai_addr))->sin_port));
		printf("\t\tai_addr.sin_addr:%-s\n", inet_ntoa(((struct sockaddr_in*)(prp->ai_addr))->sin_addr));

		res = pthread_create(&tid[i++], NULL, sock_thread, prp);
		if (res)
		{
			printf("create thread fail:%d\n", errno);
		}
	}
	
	for (i = 0; i < sizeof(tid) && 0 < tid[i]; ++i)
	{
		pthread_join(tid[i], NULL);
	}	

	return 0;
}
#endif

#if defined(ADDR_INFO_CLIENT)
int main(int argc, char* argv[])
{
	int sfd = -1;
	int res = -1;
	struct sockaddr_in sin;
	struct timespec mts,nts;
	char buf[12] = "1234567890";

	if (argc < 3)
	{
		printf("arg few\n");
		return -1;
	}

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
	{
		perror("socket");
		return -1
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(atoi(argv[1]));
	inet_pton(AF_INET, argv[2], &sin.sin_addr);

	res = connect(sfd, (struct sockaddr*)&sin, sizeof(sin));
	if (res < 0)
	{
		perror("connect");
		return -1;
	}

	mts.tv_sec = 1;
	mts.tv_nsec = 200000000;
	do
	{
		res = write(sfd, buf, strlen(buf));
		printf("write buf errno:%d\n", res < 0 ? errno : 0);
		nanosleep(&mts, &nts);

	} while(1);

	return 0;
}

#endif
