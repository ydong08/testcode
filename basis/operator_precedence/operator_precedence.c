#include <stdio.h>
#include <stdlib.h>

int main() {
  int base = 10;
  int num = 11;
  int multi = num*base++;
  printf("pre-multi: %d\n", multi);
  printf("multi: %d\n", num*base++);
  /*################表达式计算################*/

  char *ptr = NULL;
  char name[32] = {"Beijing TianAnMen"};
  const char *chairman = "Mao Ze Dong";

  if (NULL != (ptr = name, chairman))
  {
	printf("str: %s.\n", ptr);
  }
  printf("End line.\n");

  return 0;

}
