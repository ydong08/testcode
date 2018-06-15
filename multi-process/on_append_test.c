

/**
 * o_append_text.rar里面的测试结果数据文件大小是test.rar里面的2倍。
 * 分析其原因，是因为函数调用fork时，在父子进程中没有为他们采取必要的同步措施，
 * 因此在写文件时有竞争发生，导致结果混乱。
 * 另外，文件大小的不同是因为，文件表项中的文件偏移量的值类型并不是一个易失形变量类型，
 * 从而导致在写文件时读取的偏移值变量的值不是最新的值，从而导致文件大小会不同的结果。
 * 另外，从结果数据中（可能数据没有充分表现出如下所说的情况，
 * 但是你可以通过调整测试程序里的参数，并多运行几次测试程序就可以得到如下所述的结果）
 * 可以得出：当以O_APPEND标志打开文件时，write将执行原子操作，read亦然。
 * 而没有使用O_APPEND标志打开文件时，父子进程的数据输出将出现乱序的情况。
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {

	int a = 1<<12, b = 1<<12;
	int i = 0;

	char *a_array = malloc(a);
	char *b_array = malloc(b);

	memset(a_array, 'a', a);
	memset(b_array, 'b', b);

	int fd = open("ab.txt", O_WRONLY);

	if (fd == -1) {
		perror("open()");
		return -1;
	}

	switch (fork()) {
	
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

	free(a_array);
	free(b_array);

	close(fd);

	return 0;
}
