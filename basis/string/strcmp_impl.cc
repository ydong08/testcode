#include<stdio.h>
#include<assert.h>
int mystrcmp(const char* pstr1, const char* pstr2) {
   assert((pstr1!=NULL)&&(pstr2!=NULL));
   int ret=0;
   /*&&pstr2 ��֤���ַ�������������ѭ��*/
   while(!(ret=*(unsigned char*)pstr1-
               *(unsigned char*)pstr2)&&*pstr2) {
      pstr1++;
      pstr2++;
   }   
   if(ret>0)
      ret=1;
   else if(ret<0)
           ret=-1;
   return ret;
}
int main() {
   /*�ַ���Ĭ���ֵ���*/
   char* str="ABC";
   char* str2="ABCD";
   int res=mystrcmp(str,str2);
   if(res==1)
   printf("str>str2\n");
   else if(res==-1)
           printf("str<str2\n");
         else
            printf("str=str2\n");
   return 0;

