#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
	const char *dev = "breth8";
	char       mac[8] = {0};
	char       ipstr[INET_ADDRSTRLEN] = {0};
	struct ifreq ifr;
	uint32_t ipnum;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		printf("socket\n");
		return -1;
	}

	
	memset(&ifr, 0, sizeof(ifr));
	ifr.ifr_addr.sa_family = AF_INET;
	snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", dev);
	
	if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0)
	{
		printf("ioctl\n");
		return -1;
	}

	//memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);

//	printf("mac: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1],mac[2], mac[3],mac[4],mac[5]);
	printf("mac: %02X:%02X:%02X:%02X:%02X:%02X\n", 
									(unsigned char)ifr.ifr_hwaddr.sa_data[0], 
									(unsigned char)ifr.ifr_hwaddr.sa_data[1], 
									(unsigned char)ifr.ifr_hwaddr.sa_data[2], 
									(unsigned char)ifr.ifr_hwaddr.sa_data[3], 
									(unsigned char)ifr.ifr_hwaddr.sa_data[4], 
									(unsigned char)ifr.ifr_hwaddr.sa_data[5]);

	
	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
		printf("ioctl\n");
		return -1;
	}
	
	memcpy((void *)&ipnum, (void *)&ifr.ifr_addr.sa_data + 2, 4);
	printf("ipnum:%#X\n", ipnum);
	const char *pos = inet_ntop(AF_INET, &ipnum, ipstr, sizeof(ipstr));
	if (!pos)
		perror("inet_ntop");
	printf("ipstr:%s\n", ipstr);
	return 0;

}
