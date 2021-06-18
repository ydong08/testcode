#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CFG_MAX_NODE_LEVEL 64
#define CFG_TYPE_SHIFT     16

int main()
{
	char objpath[64] = "root.wan.pvc.2.entry.3";
	char *token = NULL;
	char *ptr = objpath;
	char *saveptr = NULL;
	int level= 0,need_alpha = 1,digit = 0,oid = 100,tid = 0;
	int objid[CFG_MAX_NODE_LEVEL] = {0};

	token = strtok_r(ptr, ".", &saveptr);
	while(token && level < CFG_MAX_NODE_LEVEL)
	{
		digit = 0;
		if (token[0] >= '1' && token[0] <= '9')
			digit = atoi(token);

		if (need_alpha && digit)
			return -1;

		token = strtok_r(NULL, ".", &saveptr);

		if (digit)
		{
			oid = digit;
			need_alpha = 1;
			continue;
		}

		tid = 1;
		oid = oid == 100 ? 100 : oid;
		need_alpha = 0;
		printf("oid:%d.\n", oid);
		if (oid && tid)
			objid[level++] = (tid << CFG_TYPE_SHIFT) | (oid == 100 ? 1 : oid);
	}

	if (token || level == CFG_MAX_NODE_LEVEL)   
		printf("token NOT null or level exceed limit.\n");

	for (tid = 0; tid < level; ++tid)
	{
		printf("objid[%d]: %d.\n", tid, objid[tid]);
	}

	return 0;
}
