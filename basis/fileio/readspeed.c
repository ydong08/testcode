#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	const char *file = "speed.txt";
	char  *ptr = NULL;
    char  *endptr = NULL;
    int flag = 0;
    char info[4096] = {0};
	if (argc<2)
		return 0;

	flag = atoi(argv[1]);
    FILE *fp = fopen(file, "r");
    if (fp)
    {
        fread(info, sizeof(info) - 1, 1, fp);
        fclose(fp);
        if (0 == flag)
        {
            if (NULL != (ptr = strstr(info, "Average Rate(KB/s):")))
            {
                endptr = ptr + 20;
                if (isspace(*endptr))
                {
                    endptr++;
                }

                while(isdigit(*endptr)) 
                    endptr++;

                *endptr = 0;
                printf("[%s:%d] Average Rate:%s.\n", __FUNCTION__, __LINE__, ptr + 20);
                //return (float)(8 * atoi(ptr + 20))/1024;
            }
        }
        else if (1 == flag)
        {
            if (NULL != (ptr = strstr(info, "speed(KByte/s):")))
            {
                endptr = ptr + 15;
                if (isspace(*endptr))
                {
                    endptr++;
                }

                while(isdigit(*endptr)) 
                    endptr++;

                *endptr = 0;
                printf("[%s:%d] Current Rate:%s.\n", __FUNCTION__, __LINE__, ptr + 15);
                //return (float)(8 * atoi(ptr + 20))/1024;
            }
        }
        else if (2 == flag)
        {
            if (NULL != (ptr = strstr(info, "Peak Rate (KB/s):")))
            {
                endptr = ptr + 17;
                if (isspace(*endptr))
                {
                    endptr++;
                }

                while(isdigit(*endptr)) 
                    endptr++;

                *endptr = 0;
                printf("[%s:%d] Peak Rate:%s.\n", __FUNCTION__, __LINE__, ptr + 17);
                //return (float)(8 * atoi(ptr + 18))/1024;
            }
        }
		else if (3 == flag)
		{
			char file[32] = {0};
			if (NULL != (ptr = strstr(info, "\"download\":\"")))
			{
				printf("downlaod \n");
				ptr += strlen("\"download\"=\"");
				if (NULL != (endptr = strstr(ptr, "\"")))
				{
					printf("downlaod value\n");
					strncpy(file, ptr, endptr - ptr);
					printf("file:%s\n", file);
				}
			}
		}
    }

	//snprintf(info, sizeof(info), "\"name\"=\"%s\"", "china");	
	//printf("msg:%s\n", info);

    return 0;
}
