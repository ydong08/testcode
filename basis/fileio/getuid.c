#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
 
int main(void)
{
    printf ("The real user ID is: %d\n", getuid());
    printf ("The effective user ID is :%d\n", geteuid());
    return (0);
}

