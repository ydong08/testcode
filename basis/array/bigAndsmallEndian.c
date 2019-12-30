#include <stdio.h>
#include <stdlib.h>

typedef union bigDian
{
	unsigned char by[2];
	unsigned short int i;
} Dian;

int main()
{
	Dian da;
	da.by[0] = 0x12;
	da.by[1] = 0x34;
	//da.i = 0x1234;
	printf("int len: %d\n", sizeof(unsigned short int));
	printf("1st: %02x\n", da.i);
	return 0;

}
