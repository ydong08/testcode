#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "shm.test"

int main()
{
    /* �����������,���Բ鿴/dev/shmĿ¼ */
    int fd = shm_open(FILENAME, O_RDONLY, 0);
    if (fd == -1) {
        perror("open failed:");
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
    char *ptr = (char*)mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed:");
        exit(1);
    }
    printf("mmap %s success\n", FILENAME);
    close(fd); /* �ر��׽��� */
    printf("the read msg is:%s\n", ptr);
    sleep(30);
    return 0;
}