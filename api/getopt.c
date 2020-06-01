
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int opt = -1;
	int flag = -1;
	int found = -1;
	int sec = -1;

	while ((opt = getopt(argc, argv, "nt:")) != -1)
	{
		switch(opt)
		{
			case 'n':
				flag = 1;
				break;

			case 't':
				found = 1;
				sec = atoi(optarg);
				break;

			default:
				printf("optind: %d\n", optind);
				printf("opterr: %d\n", opterr);
				printf("optopt: %c\n", optopt);
		}
	}

	printf("flag = %d; found = %d; sec = %d; optind = %d, argc = %d\n", flag, found, sec, optind, argc);
	if (argc < optind)
	{
		printf("argc < optind\n");
	}

	printf("last invalid argument:%s\n", argv[optind]);

	return 0;
}