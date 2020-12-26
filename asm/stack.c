#include <stdio.h>

int tfn(char c)
{
	printf("%c\n", c);
}

int pfn(int x, int y)
{
	return x + y;
}


int main()
{
	char c='a';
	int x,y,z;

	x = 1;
	y = 2;

	tfn(c);
	z = pfn(x, y);
	printf("%d=%d+%d\n", z, x, y);
	
	return  0;
}
