#include <stdio.h>
#include <string.h>


typedef struct Pointer {
	char* name;
	int* age;
} POINTER, *PPOINTER;

typedef struct stPoint
{
	char name[32];
	char age[4];

} STPOINT, *PSTPOINT;

int main() {

	STPOINT stp;
	strcpy(stp.name, "123456789");
	strcpy(stp.age, "789");
	printf("\n\tname: %s\n\tage: %s\n", stp.name, stp.age);
	strcpy(stp.name, "");
	strcpy(stp.age, "");
	printf("\n\tname: %s\n\tage: %s\n", stp.name, stp.age);
	PPOINTER p;
	if (p->name)
		printf("name pointer\n");
	if (p->age)
		printf("age pointer\n");

	return 0;
}
