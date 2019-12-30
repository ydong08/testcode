

#include <stdio.h>
#include <string.h>

int str_test(void)
{
    const char* pSrc = "abcdefghijk";
    const char* pSub = "hji";
    char *p = strstr(pSrc, pSub);
    if (NULL == p)
    {
        printf("not find\n");
    }
    else
    {
        int index = p - pSrc;
        printf("%s\n", p);
        printf("index: %d\n", index);
        printf("%s\n", pSrc + index);
    }

    return 0;
}

