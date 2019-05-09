/************************************
 *
 *实验验证addr[][10]这个二维数组的内存大小
 * 以及和C99支持的柔性数组的关系
 *
 *
 ************************************/

#include <stdio.h>
#include <stdlib.h>

#define GROUP_MAX_NUM      10
#define ADDR_MAX_LEN       10

typedef struct _GroupInfo
{
	int num[GROUP_MAX_NUM];
	int addr[][ADDR_MAX_LEN];
} GROUPINFO;

int main() {
	
	printf("group struct size: %d\n", sizeof(GROUPINFO));
	return 0;

}




