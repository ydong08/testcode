#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(const char* p)
{
    printf("char**: %010X\n", &p);
    printf("char*: %010X\n", p);
    printf("char*: %c\n", *p);
    return;
}


int while_loop()
{
    const char* ptrName = "castles";
    const char* ptrNode = "castles";
    memcmp(ptrName, ptrNode, strlen(ptrName));
      printf("ret: %d\n"  , memcmp(ptrName, ptrNode, strlen(ptrName)));
      printf("ret: %d\n"  , memcmp(ptrName, ptrNode, 0));
    printf("ret: %d\n"  ,! memcmp(ptrName, ptrNode, strlen(ptrName)));
     printf("ret: %d\n", ~ memcmp(ptrName, ptrNode, strlen(ptrName)));
     printf("ret: %d\n" , ! memcmp(ptrName, ptrNode, strlen(ptrName)));
     printf("ret: %d\n", ~ memcmp(ptrName, ptrNode, strlen(ptrName)));

    printf("char**: %010X\n", &ptrName);
     printf("char**: %010X\n", &ptrNode);
    printf("char*: %010X\n", ptrName);
    
    printf("char*: %c\n", *ptrName);
    print(ptrName);

    int nLen = 0;
    unsigned char* pstr = "\0x01\0x02\0x03";
    unsigned char i = 3;
    for (; i > 0; --i)
    {
        printf("single: %d\n", pstr[i-1] * pow(256, (3 - i)));
        nLen += pstr[i-1] * pow(256, (3 - i));
        printf("add: %d\n", nLen);
    }
    return 0;
}