#include <stdio.h>
#include <stdlib.h>



int portalWriteUrl(const char * buff)
{
	char portalUrl[512] = {0};
	char result[4] = {0};
	FILE * fp = NULL;
	char clientIp[64] = {0};
	char * str = NULL;

	if(NULL == buff)
	{
		return -1;
	}
	
	if(NULL != (str = strstr(buff,"\"Result\"")))
	{
		result[0] = str[9];
		printf("\nstr:%s  result:%s\n", str,result);
	}	

	if(0 != strcmp(result,"0"))
	{
		printf("\nresult error\n");
	}
	printf("\nbuff:%s\n", buff);

		
	printf("\nstr:%s\n", str);
#if 1
	fp = fmemopen(buff, strlen(buff), "r");
	if (!fp)
	{
		//sscanf(str,"\"Url\":\"%[^\"]",portalUrl);
	}
	else
	{
		if (EOF == fscanf(fp, "\"Url\":\"%[^\"]",portalUrl))
		{
			//sscanf(str,"\"Url\":\"%[^\"]",portalUrl);
		}
	}

	printf("\nstr:%s\n", str);
	if (!fp)
	{
		//sscanf(str,"client_mac=%[^\&]",clientIp);
	}
	else
	{
		if (EOF == fscanf(fp, "client_mac=%[^\&]", clientIp))
		{
			//sscanf(str,"client_mac=%[^\&]",clientIp);
		}
	}

	fclose(fp);
#else
	sscanf(str,"\"Url\":\"%[^\"]",portalUrl);
	sscanf(str,"client_mac=%[^\&]",clientIp);
#endif
	return 0；

}

int main()
{
	const char *p = "{\"Url\":\"http://112.35.144.16:9010/wifi/portal/login_p.jsp?gw_sn=ITIB90183C0F&gw_mac=D8AE90183C0F&client_mac=10:2a:b3:96:5d:d3&v=MDQ2Y2VjYmFlOWYwYTU5YmYxNTJhMDVkNzUyYzk5NDU4MmU1OWE0Yzk4ZWU2YjJjNTIzZTNkYzVmMmJiNWEzMDgyY2QzMzE5ZWJmNTlhNWNhNTYzM2ExMDI2MWM1OGM0YjY2ODQwZTY2NWU0ZDJjM2YzMzY5OTllYjczMjZkZGE5NjdkNzY2MTVhZTk1OTU4YzA5MDg5Mjg0N2Y5OGRjMjg4NDU1NzkwYThmY2Y5YjRiOTgwN2RkMjI1ZGQ4NzM1NTJjNDhlYTBkZjgwYzE2Yg==&session=&uurl=http%3A%2F%2Fwww.chinamobile.com\";\"Result\":0}";
	return portalWriteUrl(p);
}
