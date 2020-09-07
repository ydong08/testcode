#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{
	struct sockaddr *addr;     /* input */
    socklen_t addrlen;         /* input */
	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

	// getnameinfo(addr, addrlen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0;
    if (getnameinfo(addr, addrlen, hbuf, sizeof(hbuf), NULL, 0, NI_NAMEREQD))
        printf("could not resolve hostname");
    else
        printf("host=%s\n", hbuf);

	return 0;
}
