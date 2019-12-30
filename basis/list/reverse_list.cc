#include <stdio.h>
#include <malloc.h>
typedef struct Node{
	int data;
	struct Node* next;	
} Node;

Node* create(void) {
	Node* head, *tail, *q;
	int x;
	printf("please input a num: ");
	scanf("%d",&x);
	tail = (Node*)malloc(sizeof(Node));
	tail -> data = x;
	head = tail; // 头指针
	q = head;
	while(1){ // 新建后面的节点 
		printf("please input a num: ");
		scanf("%d",&x);
		if(x == 0)
			break;
		tail = (Node*)malloc(sizeof(Node));
		tail -> data = x;
		q -> next = tail;
		q = tail;
	}
	q -> next = NULL;
	return head;
}
Node* Merge(Node* head1, Node* head2){
	if(!head1) return head2;
	if(!head2) return head1;
	Node* head, *p1, *p2;
	if(head1 -> data > head2 ->data){
		head = head2;
		p1 = head1;
		p2 = head2 ->next;
	}else {
		head = head1;
		p1 = head1 -> next;
		p2 = head2;
	}
	Node* pc = head;
	while( p1 && p2){
		if(p1 ->data < p2 ->data){
			pc ->next = p1;
			pc = p1;
			p1 = p1 ->next;
		}else{
			pc -> next = p2;
			pc = p2;
			p2 = p2 ->next;
		}
	}
	if(!p1) pc -> next = p2;
	if(!p2) pc -> next = p1;
	return head;
}

Node* reverse(Node* head){
	Node* p, *q, *r;
	//用变量p获取链表头节点,实质获取整个链表
	//p->next->next->next...可表示该链表的任意节点
	p = head;
	//获取第二个节点地址
	q = p -> next;
	//反转后，头节点为尾节点，其指针必然为空
	p -> next = NULL; // 头成尾,头节点与之后节点连接截断 
	while(q){
		//获取第三节点地址，准备循环
		r = q -> next;
		//此语句可分为2句:q->next=NULL;q->next=p;
		//第二节点指针置空,p赋值后指向原头节点(p一直保存头节点)
		//实现首次反转,此时q为头节点
		q -> next = p;
		//将新的头节点(q值)保存在p中,q值释放成为自由变量
		p = q;
		//将第三节点地址(r值)保存在q中(q一直保存新节点地址),
		//r值释放成为自由变量而后得到后一节点地址:
		//r=q->next;实现循环
		q = r;
	}
	//将反转后的链表头节点保存在head中
	head = p;
	return head;
}

void print(Node* head) {
	Node* p = head;
	while(p){
		printf("%d ",p -> data);
		p = p -> next;
	}
	printf("\n");
}


