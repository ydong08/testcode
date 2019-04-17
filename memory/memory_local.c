
#include<stdio.h>
#include <stdlib.h>

#define MAX_LEN  (10)

int main() {
  char* p = NULL;
  /* memory leak */
  p = (char*)malloc(sizeof(char)*MAX_LEN);
  
  return 0;
}
