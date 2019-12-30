//#include<iostream>
#include<stdio.h>
#include<stdlib.h>
//using namespace std;
/*
void output(int (&a)[4]){
	for(int i=0; i<4; i++){
		cout<<a[i]<<endl;
	}
}

int main(int argc, char* argv[]){
	int a[4]={0};
	output(a);
	getchar();
	return 0;
} 
*/

int main(){
	int ia =1;
	int &ib = ia;
	int ic = 2;
	printf("%p\n",&ia);
	printf("%p\n",&ib);
	printf("%p\n",&ic); 
	return 0;
}



