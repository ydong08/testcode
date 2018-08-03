#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int file_test(int argc, char **argv) {

	int a = 1<<12, b = 1<<12;
	int i = 0;
	pid_t pid;

	char *a_array = malloc(a);
	char *b_array = malloc(b);

	memset(a_array, 'a', a);
	memset(b_array, 'b', b);

	int fd = open("ab.txt", O_WRONLY);

	if (fd == -1) {
		perror("open()");
		return -1;
	}

	switch (pid = fork()) {
	
		case -1:
			perror("fork() error");
			return -1;
		case 0:
			printf("parent\n");
			for ( i=0; i < a; i += 64){
				if (-1 == write(fd, a_array+i, 64)) {
					perror("parent write");
				}
			}
			break;
		default:
			printf("child\n");
			for ( i=0; i<a; i += 64) {
				if (-1 == write(fd, b_array+i, 64)) {
					perror("child write");
				}
			}
			break;
	}


	if (0 == pid)
	{
		free(a_array);
		exit(0);
	}

	if (0 < pid){
		wait();
		free(b_array);
		close(fd);
		exit(0);
	}

}
