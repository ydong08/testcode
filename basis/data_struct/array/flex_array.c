/************************************
 *
 *实验验证addr[][10]这个二维数组的内存大小
 * 以及和C99支持的柔性数组的关系
 *
 *
 ************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROUP_MAX_NUM      10
#define ADDR_MAX_LEN       10

typedef struct _GroupInfo
{
	int flag;
	int num[GROUP_MAX_NUM];
	int addr[][ADDR_MAX_LEN];
} GROUPINFO;

int main() {
	GROUPINFO stGInfo;
	memset(&stGInfo, 0, sizeof(GROUPINFO));
	printf("group struct size: %d\n", sizeof(GROUPINFO));
	printf("group struct var size: %d\n", sizeof(stGInfo));
#if 0
	for (int i = 0; i < GROUP_MAX_NUM; ++i) {
		for (int j = 0; j < ADDR_MAX_LEN; ++j) {
			stGInfo.num[i] = i;
			stGInfo.addr[i][j] = i*j;
		}
	}
	
	printf("group struct var size: %d\n", sizeof(stGInfo));
#endif
	printf("addr:%p.\n", ((GROUPINFO*)0)->flag);

	typeof(((GROUPINFO*)0)->flag) type = -1;
	printf("addr:%p-%d.\n", type,type);
	return 0;

}




