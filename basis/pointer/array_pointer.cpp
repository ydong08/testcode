#include<stdio.h>
#include<iostream>

// ����û�з���ֵ��Ҫvoid����
/*
void func(void){
	printf("\nVoid usage\n");
} 

*/
int main(int argc, char* argv[]){
	/*
	int m=1;
	int* pm=&m;
	// ֻ�ǰ� m �� �͵�ַ ��һ���ֽڵĵ�ַ ��ֵ���� ptr 
	char* ptr=(char*)pm;
	
	printf("*ptr=%d\n",*ptr);// ��ȡ����m�����͵�ַ�ϵĶ����� 
	printf("ptr=0x%0x\n",ptr);
	
	if(1 == *(char*)&m)
		printf("Little Endian\n");
	else
		printf("Big Endian\n");
		
	union Endian{
		int a;
		char b;
	} end;
	
	end.a=1;
	if(1 == end.b)
		printf("Little Endian\n");
	else
		printf("Big Endian\n");
	func();
	
	int pint;
	printf("size=%d\n",sizeof pint);
	pint;
	printf("size=%d\n",sizeof pint);
	
	class voids{
	public:
		voids(int a, char b):nu(a),chr(b){}
		//print(int num){
		//	printf("%d",num);
		//}
		
	//private:
		int nu;
		char chr;
	};
	voids vo(21,'A');
	printf("Add= %d\n",&vo);
	printf("Add= %d\n",&(vo.nu));
	printf("Addr= %d\n",(void*)vo.nu);
	*/
	int a[4]={1,2,3,4};
	int* ptr1=(int*)(&a+1);
	int* ptr2=(int*)((int)a+1);
	int* ptr3=(int*)(a+1);
	int c=*(a+1);
	
	printf("ptr1[-1]=%x, ptr1[-2]=%x, *ptr2=%x, *ptr3=%x, %x, %x\n", ptr1[-1], ptr1[-2], *ptr2, *ptr3, c, a);
	
	return 0;	
} 


//