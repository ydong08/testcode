
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/route.h>

#define INET_ADDR(x)	(((struct sockaddr_in *) &(x))->sin_addr.s_addr)

int main()
{
	const char *gw = "172.168.41.254";
	int ipfd = -1;
	struct rtentry rt;

	memset(&rt, 0, sizeof(rt));
    rt.rt_dst.sa_family = AF_INET;
    INET_ADDR(rt.rt_dst) = 0; 
    rt.rt_gateway.sa_family = AF_INET;
    INET_ADDR(rt.rt_gateway) = inet_addr(gw); 
    rt.rt_flags = RTF_GATEWAY;

    //ipfd = open(mux_dev_name, O_RDWR, 0);
    ipfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (ipfd < 0)
    {
    	perror("open");
    	exit(1);
    }

    if (ioctl(ipfd, SIOCADDRT, &rt) < 0) { 
    	perror("ioctl");                                                                                                                                            
    	return 1;
    }    


    return 0;

}

