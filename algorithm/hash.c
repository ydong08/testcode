#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NODE_NUM   (16)
#define KEY_VALUE_SIZE (64)
#define KEY_DEPTH_SIZE (32)
#define MAKE_LIST_SIZE (16)                          

typedef struct make_node_s {                                                                                                                                                         
    char value[KEY_VALUE_SIZE];
    struct make_node_s *dnext; //depth list
} make_node_t;

typedef struct make_list_s {
    struct make_node_s *list[MAKE_LIST_SIZE];
} make_list_t;

void clear_hash_list(make_list_t* hmhead)
{
	int i = 0;
	make_node_t* node = NULL;
	make_node_t* tmpnode = NULL;
	if (!hmhead)
		return ;

	for (i = 0; i < MAKE_LIST_SIZE; ++i) {
		if (!hmhead->list[i])
			continue;
		node = hmhead->list[i];
		while(node) {
			tmpnode = node->dnext;
			free(node);
			node = tmpnode;
		}
	}

	free(hmhead);

	return ;
}

void init_hash_head(make_list_t** hmhead)
{
	int i = 0;
	make_list_t* tmphm = NULL;
	if (!hmhead) {
		printf("hmhead is null.\n");
		return ;
	}

	printf("init_hash_head\n");
	tmphm = *hmhead;
	tmphm = (make_list_t*)malloc(sizeof(make_list_t));
	if (!tmphm) {
		printf("malloc for hmhead fail.\n");
		return ;
	}

	for (i = 0; i < MAKE_LIST_SIZE; ++i) {
		tmphm->list[i] = (make_node_t*)malloc(sizeof(make_node_t));
		if (!tmphm->list[i])
			goto clear_mem;
	}

	*hmhead = tmphm;
	return ;

clear_mem:
	clear_hash_list(tmphm);
	return ;
}

/* 0 ~ MAX_NODE_NUM number string */
void create_hash_node(make_list_t* hmhead)
{
	int i = 0;
	int j = 0;
	int total = 0;
	int num = 0;
	make_node_t* tmphm = NULL;
	make_node_t* tmpnode = NULL;

	if (!hmhead)
		return ;

	printf("create_hash_node\n------------------------\n");
	for (i = 0; i < MAKE_LIST_SIZE; ++i)
	{
		if (!hmhead->list[i])
			return ;

		tmphm = hmhead->list[i];
		num = (i+1) * 100;
		srandom(time(0));
		total = random()%MAX_NODE_NUM;

		for(j = 0; j < total; ++j)
		{
			tmpnode = (make_node_t*)malloc(sizeof(make_node_t));
			if (!tmpnode)
				return ;

			snprintf(tmpnode->value, sizeof(tmpnode->value), "%d", num++);
			tmphm->dnext = tmpnode;
			tmphm = tmpnode;
			printf("--list[%02d] node[%02d] value[%4s]\n", i, total, tmpnode->value);
		}
	}

	return ;
}

void verify_hash_list(make_list_t* hmhead)
{
	int i = 0;
	make_node_t *n = NULL;

	if (!hmhead)
		return ;

	printf("verify_hash_list\n------------------------\n");
	for(i = 1, n = hmhead->list[i]->dnext; n || (++i < MAKE_LIST_SIZE && (n = hmhead->list[i]->dnext)); ) {
		if (!n) {
			return ;
		}

		printf("--list[%02d] node[xx] value[%4s]\n", i, n->value);
		n = n->dnext;
	}

	return ;
}


int main()
{
	make_list_t* hmhead = NULL;

	init_hash_head(&hmhead);
	create_hash_node(hmhead);	
	verify_hash_list(hmhead);
	clear_hash_list(hmhead);

	return 0;
}
