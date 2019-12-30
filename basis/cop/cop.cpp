#include <stdio.h>
#include <stdlib.h>

int main() {
  int base = 10;
  int num = 11;
  int multi = num*base++;
  printf("pre-multi: %d\n", multi);
  printf("multi: %d\n", num*base++);

}
