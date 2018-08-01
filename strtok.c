#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char pstr[] = "hello@world@this@is@a@test@stirng";
    char* pt = strtok(pstr, "@");
    while(pt) {
        printf("%s ", pt);
        pt = strtok(NULL, "@");
    }
    printf("\n");
    return 0;
}

