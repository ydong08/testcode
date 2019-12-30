
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stInfo
{
char a;
short int b;
int d;
char c;
} INFO;

int main() {
 INFO in;
 printf("size: %d\n", sizeof(INFO));

 char* p = malloc(100);
 if (!p)
   printf("malloc fail\n");

 INFO* stIn = (INFO*)p;
 stIn->a = 8;
 stIn->b = 234;
 stIn->d = 65538;
 stIn->c = 16;
 free(stIn);
 return 0;
	

}
