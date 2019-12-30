#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

/*
void quickSort(int data[], size_t lv, size_t rv){
	size_t p=(lv+rv)/2;
	int pivot=data[p];
	size_t i;
	size_t j;
	for(i=lv,j=rv;i<j;){
		while(!(i>=p||pivot<data[i]))
			++i;
		if(i<p){
			data[p]=data[i];
			p=i;
		}
		
		while(!(j<=p||pivot>data[j])){
			--j;
		}
		if(p<j){
			data[p]=data[j];
			p=j;
		}
	}
	data[p]=pivot;
	if(p-lv>1){
		quickSort(data,lv,p-1);
	}
	if(rv-p>1){
		quickSort(data,p+1,rv);
	}
}
*/
void Qsort(int a[], int low, int high)
{
    if(low >= high)
    {
        return;
    }
    int first = low;
    int last = high;
    int key = a[first];/*用字表的第一个记录作为枢轴*/
 
    while(first < last)
    {
        while(first < last && a[last] >= key)
        {
            --last;
        }
 
        a[first] = a[last];/*将比第一个小的移到低端*/
 
        while(first < last && a[first] <= key)
        {
            ++first;
        }
         
        a[last] = a[first];    
/*将比第一个大的移到高端*/
    }
    a[first] = key;/*枢轴记录到位*/
    Qsort(a, low, first-1);
    Qsort(a, first+1, high);
}
int main(int argc, char* argv[]){
	int data[6]={12,23,1,5,9,234};
	size_t size=sizeof(data)/sizeof(data[0]);
	Qsort(data,0,size-1);
	unsigned i;
	for(i=0;i<size;++i){
		printf("%d,",data[i]);
	}
	printf("\n\n");
}