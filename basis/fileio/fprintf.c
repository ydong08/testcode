
#include <stdio.h>
#include <stdlib.h>


int main()
{
	FILE *fp = fopen("tmp.txt", "w+");
	if (fp == NULL)
		return 0;

	fprintf(fp, "%s", "#!/bin/bash");

	fclose(fp);

	return 0;

}
