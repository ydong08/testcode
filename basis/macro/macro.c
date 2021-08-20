#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define _COLUMNS(x) "\"\"x\"\":\"\"%s\"\","

#define _LOG_LINE(x)    __FILE__##x##__line__
//#define LOG_LINE(x)     _LOG_LINE(x)
#define LOG_LINE(x)      x##"p"
//#define LOG_FILE    __FILE__##":"##LOG_LINE

#define PROCESS1   y
#define PROCESS2   x  

int main()
{
    printf("line: %s\n", LOG_LINE(":"));
    //printf("info: %s\n", LOG_FILE);

    snprintf(recv, sizeof(recv), CURL_REPORT_COLUMN("ip"), "192.168.1.1");
	printf("recv:%s\n", recv);
#if (PROCESS1 == y)
	printf("PROCESS1\n");
#elif (PROCESS2 == x)
	printf("PROCESS2\n");
#elif (PROCESS3 == z)
	printf("PROCESS3\n");
#endif
    return 0;
}

