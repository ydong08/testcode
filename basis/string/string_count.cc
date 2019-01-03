/*在父字串中统计子字串出现的次数*/

#include<stdio.h>
int main() {
      char str1[20],str2[20],*p1,*p2;
      int sum=0;
      printf("please Input two strings:\n");
      scanf("%s%s",str1,str2);
      p1=str1;
      p2=str2;
      while(*p1!='\0') {
         if(*p1==*p2) {
            while(*p1==*p2&&*p2!='\0'){
            p1++;
            p2++;
            }   
         }   
         else 
            p1++;
         /*if-else语句体执行完毕后
           要么未找到子字串
           要么已匹配完一次子字串：
           此时p2指向子字串'\0'位置
           据此可判断已找到子字串*/
         if(*p2=='\0')// 
           sum++;
         p2=str2;//将p2复位至子字串首地址
      }
   printf("出现次数:%d\n",sum);
}
      
