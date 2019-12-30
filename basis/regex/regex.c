#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

int main()
{
    int stat, errcode;
    int cflags = REG_EXTENDED;
    regmatch_t stmatch;
    memset(&stmatch, 0, sizeof(stmatch));
    regex_t reg;
    const char* pattern = "^\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*.\\w+([-.]\\w+)*$";
    char* pbuf = "winter08@126.com";
    char ebuf[128] = {0};

    regcomp(&reg,pattern, cflags);
    size_t nmatch = 1;
    stat = regexec(&reg, pbuf, nmatch, &stmatch, 0);
    printf("ret: %d\n", stat);
    
    if (0 == stat){
        memcpy(ebuf, pbuf+stmatch.rm_so,  stmatch.rm_eo - stmatch.rm_so+1);
        printf("match: %s\n", ebuf);
    }

    memset(ebuf, 0, sizeof(ebuf));
    if (REG_NOMATCH == stat ){
        printf("ret: %d\n", stat);
    } else if (0 != stat){
        regerror(errcode, &reg, ebuf, sizeof(ebuf));
        printf("Error: %d-%s\n", errcode, ebuf);
        return 1;
    }

    regfree(&reg);

    return 0;
}

