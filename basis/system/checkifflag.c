#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>


int main()
{
	const char* ifname="ppp1";
	int sockfd;
	int flag = IFF_UP;
    struct ifreq ifr; 
    
    if (ifname == NULL)
    {    
        return -1;
    }    
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {    
        return -1;
    }    
    
    memset(&ifr, 0, sizeof(struct ifreq));
	ifr.ifr_flags &= ~IFF_UP;
    ifr.ifr_flags |= IFF_POINTOPOINT;
    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name)-1);
    if ((ioctl(sockfd, SIOCSIFFLAGS, &ifr)) < 0) 
    {    
		printf("ioctl set flag error.\n");
        close(sockfd);
        return -1;
    }   

   	close(sockfd);

	return 0;

}
