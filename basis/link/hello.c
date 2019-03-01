
#include <stdio.h>

int hello_world_data = 0x1234; 
int hello_world_bss;

int hello_world( ) 
{ 
    int tmp;

    printf("hello world!\n"); 
    tmp = hello_world_data + hello_world_bss;

    return tmp; 
}
