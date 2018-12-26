/*选择排序*/
#include<iostream>
using namespace std;
void selectSort (int data[], size_t size) {
  for (size_t i = 0; i < size - 1; i++) {
    size_t min = i;
    for (size_t j = i + 1; j < size; j++)
      if (data[j] < data[min])
	    min = j;
    if (min != i) {//最小位置不是自己
      int temp = data[i];
      data[i] = data[min];
      data[min] = temp;
    }
  }
}


int main(void) {
  int data[] = {12,78,54,65,92,13,68,61,123,99};
  int size = sizeof(data)/sizeof(data[0]);
  selectSort (data, size);
  for (size_t m = 0; m < size - 1; m++)
    cout << data[m] << ' ';
  cout << endl;
  return 0;
}


