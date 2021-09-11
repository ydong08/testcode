#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	PINPAI,
	ZHILIANG
} SANMU;

int main()
{
	char vlanset[128] = {"20/30/40/50/60"};
	char vlan[12] = {"30"};
	char vlansettmp[128] = {0};

	char awkbuf[512] = {0};

	char* pos = strstr(vlanset, vlan);
	//memset(pos, 0, strlen(vlan)+1);
	*pos = '\0';
	strncpy(vlansettmp, vlanset, strlen(vlanset));
	strcat(vlansettmp, pos+strlen(vlan)+1);
	printf("vlansettmp: %s\n\n", vlansettmp);
	
	if (!strcmp(vlansettmp, vlan))
		printf("equal\n");
	else 
		printf("not equal\n");
	
	return 0;

}
