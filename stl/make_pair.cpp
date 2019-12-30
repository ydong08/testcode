#include <iostream>
#include <typeinfo>
using namespace std;

/**
	std::pair主要的作用是将两个数据组合成一个数据，两个数据可以是同一类型或者不同类型。
例如std::pair<int,float> 或者 std：：pair<double,double>等。
pair实质上是一个结构体，其主要的两个成员变量是first和second，这两个变量可以直接使用。
初始化一个pair可以使用构造函数，也可以使用std::make_pair函数，make_pair函数的定义如下：
template pair make_pair(T1 a, T2 b) { return pair(a, b); }

    一般make_pair都使用在需要pair做参数的位置，可以直接调用make_pair生成pair对象。 
另一个使用的方面就是pair可以接受隐式的类型转换，这样可以获得更高的灵活度。但是这样会出现如下问题：
	例如有如下两个定义：
		std::pair<int, float>(1, 1.1);

		std::make_pair(1, 1.1);
其中第一个的second变量是float类型，而make_pair函数会将second变量都转换成double类型。
这个问题在编程是需要引起注意。
下面是一段pair与make_pair的例子程序：

*/
int main(int argc, char** argv)
{
	pair <string,double> product1 ("tomatoes",3.20);
	pair <string,double> product2;
	pair <string,double> product3;
	pair<int, float> product4;
	
	product2.first ="lightbulbs"; // type of first is string
	product2.second =1.00; // type of second is double
	
	product3 = make_pair ("shoes",20.00);
	product4 = make_pair(10, 1.12);
	
	cout <<"The price of "<< product1.first <<" is $"<< product1.second <<"\n";
	cout <<"The price of "<< product2.first <<" is $"<< product2.second <<"\n";
	cout <<"The price of "<< product3.first <<" is $"<< product3.second <<"\n";
	
	cout <<"The price of "<< product4.first <<" is $"<< product4.second <<"\n";
	cout<<"product1 second type: " << typeid(product1.second).name() << endl;
	cout<<"product2 second type: " << typeid(product2.second).name() << endl;
	cout<<"product3 second type: " << typeid(product3.second).name() << endl;
	cout<<"product4 second type: " << typeid(product4.second).name() << endl;

	return 0;
}

