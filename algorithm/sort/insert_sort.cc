
/*插入排序*/

#include<iostream>
using namespace std;
void insertSort (int data[], size_t size) {	
	size_t i;
	int temp;
	for (i = 1;i<size;i++) {
		temp = data[i];	
		size_t j;
		for (j=i;j>0&&temp<data[j-1];j--)	//
			data[j] = data[j-1];
		if (j != i)
			data[j] = temp;
	}
}
int main(void) {
	int data[] = {12,23,1,2,0,35,79,9,81};
	size_t size = sizeof(data)/sizeof(data[0]);
	insertSort (data, size);
	for (size_t i = 0; i < size; i++)
		cout << data[i] << endl;

	return 0;
}
