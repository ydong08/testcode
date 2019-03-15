/*将指定整数转换为字符串*/
#include<stdio.h>
#include<stdlib.h>
void itoa(int num, char pval[]) {
   char strval[100];
   int i,j;
   int fval=0,sval=0;
   fval=num;
   for(i=0;i<100;i++) {
      sval=fval%10;
      fval=fval/10;
      strval[i]=sval+48;
      if(fval<10) {
         i++;
         strval[i]=fval+48;
         break;
      }   
   }   
   for(j=0;j<=i;j++) 
      pval[j]=strval[i-j];
   pval[j]='\0';
}
int main() {
   int num;
   char pval[100]={0};
scanf("%d",&num);
   printf("%d\n",num);
   itoa(num,pval);
   printf("%s\n",pval);
   return 0;
}
~                 
