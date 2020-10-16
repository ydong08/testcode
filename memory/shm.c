
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#define MAXSIZE 1024*1024*16   /*�����ڴ�Ĵ�С���������ó��ڴ�ҳ��������*/
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
    sleep(30);
    /* ������ü���Ϊ0��ϵͳ�ͷ��ڴ���� */
    if (shm_unlink(FILENAME) == -1) {
        perror("shm_unlink failed:");
        exit(1);
    }
    printf("shm_unlink %s success\n", FILENAME);
    return 0;
}