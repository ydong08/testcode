#include <stdio.h>
#include <stdlib.h>

int main() {
	double a = 1/3;
	printf("a=%lf\n", a);
	float f = 1/3;
	printf("a=%f\n", f);
	char p[128] = "nas100";
	printf("value:%d\n", atoi(&p[0]));
	printf("value:%d\n", atoi(&p[2]));
	printf("value:%d\n", atoi(&p[3]));
	return 0;
}
