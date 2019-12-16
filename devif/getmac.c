#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GET_NEIGH_CMD   "/bin/ip neigh show dev eno1"
#define CLIENT_IP       "192.168.0.126"
int main()
{
	int ret = 0;
	char *endp = NULL;
	char* p = NULL;
	char *tmp = NULL;
    char *start, *end;
	char buf[128] = {0};
	char mac[32] = {0};
	FILE* fp = popen(GET_NEIGH_CMD, "r");	
	if (fp) {
		while(fgets(buf, sizeof(buf), fp)) {
			if (0 == strncmp(buf, CLIENT_IP, strlen(CLIENT_IP))) {
				printf("neigh: %s\n", buf);
				p = strtok_r(buf, "lladdr", &endp);
				if (!p)
					continue;
				p = strtok_r(NULL, "STALE", &endp);
				if (!p)
					continue;		
				tmp = strtok_r(p, " ", &endp);
				tmp = strtok_r(NULL, " ", &endp);
				if (tmp)
					memcpy(mac, tmp, strlen(tmp));
			} 
			memset(buf, 0, sizeof(buf));
		}
		pclose(fp);
	}
	printf("mac: %s\n", mac);

	/* get kernel version */
    fp = fopen ("/proc/version","r");
    if(!fp){
        printf("Failed to open /proc/version\r\n");
        return 1;
    }    
	memset(mac, 0, sizeof(mac));
	ret = fscanf(fp, "%*s %*s %16[a-z0-9.-] %*s %*s", mac);
	if (ret < 0)
		perror("fscanf");
	fclose(fp);
	printf("version: %s\n", mac);
	/* get usb ko */
	fp = fopen("/proc/modules", "r");
	if (!fp)
		return 1;
	memset(mac, 0, sizeof(mac));
	start = 0;
	end = 0;	
	while (!feof(fp) ) {
		fgets(buf, sizeof(buf), fp);
		ret = sscanf(buf, "%16[a-zA-Z0-9_] %*s %*s %*s %*s %*s", mac);

		printf("modules: %s: %d\n", mac, ret);
		if (0 == strncmp(mac, "usbcore", strlen(mac)))
			start = 1; 
		if (0 == strncmp(mac, "usb_common", strlen(mac)))
			end = 1;
		if (start && end)
			break;
		
	}
	fclose(fp);

	return 0;
}

