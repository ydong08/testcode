#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;


int main()
{

  for (int i =1;i <5; ++i)
  {
  /*static */int a = i;
  //a=i;
  cout << a <<endl;
  }
 return 0;
}
