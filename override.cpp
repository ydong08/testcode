#include<iostream>
using namespace std;

class A{
public:
	void foo(){
		printf("1\n");
	}	
	
	virtual void fun(){
		printf("2\n");
	}	
};

class B : public A{
public:
	void foo(){
		printf("3\n");
	}
	
	void fun(){
		printf("4\n");
	}
};

int main(int argc, char* argv[]){
	A a;
	B b;
	A* p=&a;
	p->foo();
	p->fun();
	p=&b;  //子类指针赋值给基类指针 
	p->foo();
	p->fun();
	
	b.foo();
	b.fun(); 
	
	printf("\n\n"); 
	B* ptr=(B*)&a;
	ptr->foo();
	ptr->fun();
	return 0;
}

//