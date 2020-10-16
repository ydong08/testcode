#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 1024*4   /*�����ڴ�Ĵ�С���������ó��ڴ�ҳ��������*/
#define FILENAME "shm.test"

int main()
{
    /* �����������,���Բ鿴/dev/shmĿ¼ */
    int fd = shm_open(FILENAME, O_CREAT | O_TRUNC | O_RDWR, 0777);
    if (fd == -1) {
        perror("open failed:");
        exit(1);
    }
    /* ������С */
    if (ftruncate(fd, MAXSIZE) == -1) {
        perror("ftruncate failed:");
        exit(1);
    }
    /* ��ȡ���� */
    struct stat buf;
    if (fstat(fd, &buf) == -1) {
        perror("fstat failed:");
        exit(1);
    }
    printf("the shm object size is %ld\n", buf.st_size);
    /* ����ӳ���ϵ */
    char *ptr = (char*)mmap(NULL, MAXSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
       perror("mmap failed:");
        exit(1);
    }
    printf("mmap %s success\n", FILENAME);
    close(fd); /* �ر��׽��� */
    /* д������ */
    char *content = "hello world";
    strncpy(ptr, content, strlen(content));
    sleep(30);
    return 0;
}