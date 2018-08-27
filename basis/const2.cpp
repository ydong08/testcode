#include<iostream>
#include "const.h"
using namespace std;

int main(int argc, char* argv[]){
	int sum=1000;
	cout<<"SUM= "<<sum<<"  Addr: "<<&sum<<endl; 
	cout<<"ConType= "<<ConType<<"   Addr: "<<&ConType<<endl;
	cout<<"NonConType= "<<NonConType<<"   Addr: "<<&NonConType<<endl; 
	return 0;
}

 