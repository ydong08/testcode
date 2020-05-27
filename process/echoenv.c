
#include <stdio.h>
#include <unistd.h>
extern char** environ;
int main(int argc , char *argv[])
{
    int i;
    char **ptr;
    for(ptr = environ;*ptr != 0; ptr++)
        printf("%s\n",*ptr);
    return 0;
}
