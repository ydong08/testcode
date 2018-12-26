
#include<stdio.h>

void revString(char* pstr, char str[]) {
  int i=0,j=0;
  while(pstr[i++]) ;
  i--;
  while(pstr[j]) 
	str[--i]=pstr[j++];
}


int main() {
  char* str="ABCDEFG";
  char pstr[100]={0};
  revString(str,pstr);
  printf("%s\n",pstr);
  return 0;
}


