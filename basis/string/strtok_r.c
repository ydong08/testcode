
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	const char *pstr = "0";
	char       *tmpstr = (char*)pstr;
	char       *token = NULL;
	char       *saveptr = NULL;
	char       line[12] = {0};

//	strncpy(line, pstr, sizeof(line) - 1);
	for (tmpstr; ; tmpstr=NULL)
	{
		token=strtok_r(tmpstr, ",", &saveptr);
		if (token == NULL)
			break;

		printf("token:%s\n", token);
	}

	return 0;

}
