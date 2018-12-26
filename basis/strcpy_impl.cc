#include<stdio.h>
char*  mycpy(char ptr[], char pttr[]) {
   char* temp = ptr;
   if(ptr)
      while((*ptr++=*pttr++) != '\0') ;
   return temp;
}

int main() {
char str1[10]="ABCDSF";
char str2[10]="abcdef";
printf("%s\n",mycpy(str1,str2));
}

