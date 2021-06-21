#include <stdio.h>
#include <stdlib.h>

typedef union bigDian
{
	unsigned char by[2];
	unsigned short int i;
} Dian;

void test_size(int len)
{
	printf("len size:%d.\n", len);
	Dian dan;
	printf("dan size:%d.\n", sizeof(Dian));
}

int main()
{
	int n = 0;
	int i = 0;
	Dian da;
	da.by[0] = 0x12;
	da.by[1] = 0x34;
	//da.i = 0x1234;
	printf("int len: %d\n", sizeof(unsigned short int));
	printf("1st: %02x\n", da.i);

	for (i = 0; i == 0 ? !n : n || ++i < 100; )
	{
		n = !n?i+1:n;
		printf("n : %d.\n", n);
	}

	//printf("Dian size:%d.\n", sizeof(Dian));
	test_size(sizeof(Dian));
	return 0;

}
