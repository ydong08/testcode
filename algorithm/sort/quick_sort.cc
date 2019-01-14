*快速排序*/
#include<iostream>
using namespace std;
void quickSort (int data[], size_t l, size_t r)//l,r左边界，右边界
   size_t p = (l+r)/2;
   int pivot = data[p];
   for (size_t i = l,j = r; i < j;) {
      while (!(i >= p || pivot < data[i]))
         i++;
   if (i < p) {
      data[p] = data[i];
      p = i;
   }   
   while (!(j <= p || data[j]) < pivot)
      j--;
   if (j > p) {
      data[p] = data[j];
      p = j;
   }   
   }   
   data[p] = pivot;
   if (p - l > 1)
      quickSort (data, l, p - 1); 
   if (r - p > 1)
      quickSort (data, p + 1, r); 
}
int main(void) {
   int data[] = {1,2,45,67,8,90,59,9,68};
   int size = sizeof(data)/sizeof(data[o]);
   quickSort (data, 0, size - 1);
   for (size_t i = 0; i < szie; i++)
      cout << data[i] << endl;
   return 0;
}

