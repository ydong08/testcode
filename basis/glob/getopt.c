
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int opt = -1;
	int flag = -1;
	int found = -1;
	int sec = -1;

	while ((opt = getopt(argc, argv, "nt::")) != -1)
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
				printf("opt: %c\n", opt);
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

	printf("last invalid argument:%s\n", argv[optind - 1]);

	return 0;
}

/*
 * set args -t234  可选参数必须是-t234这种紧连接格式，否则是格式错误
 * set args -n 
 */
