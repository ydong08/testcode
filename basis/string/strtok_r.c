
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	char pstr[4] = {"0"};
	char *tmpstr = (char*)pstr;
	char *token = NULL;
	char *saveptr = NULL;
	char line[128] = {"0,1,2/3,1,1,4/5,0,1,6/7"};

	for (tmpstr; ; tmpstr=NULL)
	{
		token = strtok_r(tmpstr, ",", &saveptr);
		if (token == NULL)
			break;

		printf("token:%s\n", token);
	}

	printf("---------------\n");
	tmpstr = line;
	token = strtok_r(tmpstr, ",/", &saveptr);
	while (token)
	{
		printf("token: %s.\n", token);
		token = strtok_r(NULL, ",/", &saveptr);
	}

	return 0;

}
