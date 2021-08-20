#include <stdio.h>
#include <stdlib.h>


enum tclass
{
	TCLASS_STATIC,
	TCLASS_DYNAMIC,
	TCLASS_INVALID
};

struct memory_data
{
	int addr;
	enum tclass type;
	int len;
};

int main()
{
	int sum = 1;
	char *p = NULL;
	const char *ptr = "SevenMonthSeventDayIsBlessingDay.";
	p = (char*)ptr;
	do
	{
		printf("Charactor: %c\n", *p++);
		if ('.' == *p)
			sum = 0;
	
	} while(sum);
	
	struct memory_data  md[3] = {
		[0] = {.type = TCLASS_STATIC},
		[1] = {.type = TCLASS_DYNAMIC},
		[2] = {.type = TCLASS_INVALID},
	};

	return 0;
}
