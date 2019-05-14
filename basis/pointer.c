#include <stdio.h>


typedef struct Pointer {
	char* name;
	int* age;
} POINTER, *PPOINTER;

int main() {
	PPOINTER p;
	if (p->name)
		printf("name pointer\n");
	if (p->age)
		printf("age pointer\n");

	return 0;
}
