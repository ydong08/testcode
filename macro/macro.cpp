#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define _COLUMNS(x) "\"\"x\"\":\"\"%s\"\","

#define _LOG_LINE(x)    __FILE__##x##__line__
//#define LOG_LINE(x)     _LOG_LINE(x)
#define LOG_LINE(x)      x##"p"
//#define LOG_FILE    __FILE__##":"##LOG_LINE

int main()
{
    printf("line: %s\n", LOG_LINE(":"));
    //printf("info: %s\n", LOG_FILE);

    snprintf(recv, sizeof(recv), CURL_REPORT_COLUMN("ip"), "192.168.1.1");
	printf("recv:%s\n", recv);
    return 0;
}

