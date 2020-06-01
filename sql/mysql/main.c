
#include <stdio.h>
#include <stdlib.h>
#include "ACDBAccess.h"

#define MYSQL_USER     "admin"
#define MYSQL_HOST     "localhost"
#define MYSQL_PWD      "mysql"

#define MYSQL_DB       "acmesh"
#define MYSQL_PORTS     (0)

#define CONTROLLER_MAC "1A2B3C"
int main()
{
	int  ret     = 0;
	MYSQL_RES *res  = NULL;
	char buf[32] = {0};

	ret = ACDBConnect(MYSQL_HOST, 0, MYSQL_USER, MYSQL_PWD, MYSQL_DB);
	if (ret == MYSQL_RETURN_ERROR)
	{
		printf("[Error]dbconnet fail.\n");
		return 1;
	}

	ret = get_info_controller_radio(&res, CONTROLLER_MAC, 0, 0);
	if (ret == MYSQL_RETURN_ERROR)
	{
		printf("[Error]get network fail.\n");
	}

	ret = ACDBGetFieldValue(&res, "radio_key", buf);
	if (ret == MYSQL_RETURN_ERROR)
	{
		printf("[Error]get network field fail.\n");
	}

	printf("[Info]radio_key:%s\n", buf);
	ACDBFreeMysqlRes(&res);

	ACDBDisconnect();

	return 0;

}

