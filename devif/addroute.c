
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#define INET_ADDR(x)	(((struct sockaddr_in *) &(x))->sin_addr.s_addr)

int main()
{
	const char *gw = "172.168.41.254";
	struct rtentry rt;
	memset(&rt, 0, sizeof(rt));
    rt.rt_dst.sa_family = AF_INET;
    INET_ADDR(rt.rt_dst) = 0; 
    rt.rt_gateway.sa_family = AF_INET;
    INET_ADDR(rt.rt_gateway) = inet_addr(gw); 
    rt.rt_flags = RTF_GATEWAY;

    if (ioctl(ipfd, SIOCADDRT, &rt) < 0) { 
    	perror("ioctl");                                                                                                                                            
    	return 1;
    }    


    return 0;

}

