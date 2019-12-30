/*C 的范型（向下范型,数据类型内存化）*/
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
void quickqsort (void* base, size_t l, size_t r,
   size_t size, int (*compar) (const void*,const void*)) { 
   size_t p = (l+r)/2;
   size_t i,j;
   void* pivot = malloc (size);
   memcpy (pivot, base + p*size, size);
   for (i = l, j = r; i < j;){
      while (!(i >= p || compar(pivot,base+i*size) < 0)) 
            i++;
   if(i < p) {
      memcpy (base+p*size, base+i*size, size);
      p = i;
   }   
   while (!(j <= p || compar(base+j*size,pivot) < 0)) 
      j--;
   if (j > p)
      memcpy(base+p*size, base+j*size, size);
      p = j;
   }   
}
void my_qsort (void* base, size_t nmemb,
   size_t size, int (*compar) (const void*,
   const void*)) {
   quickqsort (base, 0, nmemb - 1, size, compar);//调用
}


int cmpInt (const void* p1, const void* p2) {
   return *(int*)p2 - *(int*)p1;
}
int cmpStr (const void* p1, const void* p2) {
   return -strcmp (*(const char**)p1,
      *(const char**)p2);
}
int main (void) {
   int na[] = {13, 45, 22, 67, 88, 19, 7, 10};
   size_t size = sizeof (na[0]);
   size_t nmemb = sizeof (na) / size;
   my_qsort (na, nmemb, size, cmpInt);
   size_t i;
   for (i = 0; i < nmemb; i++)
      printf ("%d ", na[i]);
   printf ("\n");
   const char* sa[] = {
      "beijing",
      "shanghai",
      "tianjin",
      "chongqing",
      "guangzhou",
      "shanghai"
   };
   size = sizeof (sa[0]);
   nmemb = sizeof (sa) / size;
   my_qsort (sa, nmemb, size, cmpStr);
   for (i = 0; i < nmemb; i++)
      printf ("%s ", sa[i]);
   printf ("\n");
   return 0;
}

