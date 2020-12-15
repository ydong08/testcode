
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/route.h>
#include <linux/in.h>

#define INET_ADDR(x)	(((struct sockaddr_in *) &(x))->sin_addr.s_addr)

int main()
{
	const char *gw = "192.168.43.254";
	const char *hs = "192.168.43.184";
	int ipfd = -1;
	struct rtentry rt;

	memset(&rt, 0, sizeof(rt));
#if 0
    rt.rt_gateway.sa_family = AF_INET;
    INET_ADDR(rt.rt_dst) = 0; 
    INET_ADDR(rt.rt_gateway) = inet_addr(gw); 
    rt.rt_flags = RTF_UP | RTF_GATEWAY;
#else
    rt.rt_dst.sa_family = AF_INET;
    INET_ADDR(rt.rt_dst) = inet_addr(hs); 
    INET_ADDR(rt.rt_gateway) = 0; 
    rt.rt_flags = RTF_UP | RTF_HOST;
	rt.rt_dev = (char*)"nas0_2";
#endif
    ipfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (ipfd < 0)
    {
    	perror("open");
    	exit(1);
    }

    if (ioctl(ipfd, SIOCDELRT, &rt) < 0)
    //if (ioctl(ipfd, SIOCADDRT, &rt) < 0) 
    { 
    	perror("ioctl");                                                                                                                                            
    	return 1;
    }    

    return 0;
}

