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
	head = tail; // ͷָ��
	q = head;
	while(1){ // �½�����Ľڵ� 
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
	//�ñ���p��ȡ����ͷ�ڵ�,ʵ�ʻ�ȡ��������
	//p->next->next->next...�ɱ�ʾ�����������ڵ�
	p = head;
	//��ȡ�ڶ����ڵ��ַ
	q = p -> next;
	//��ת��ͷ�ڵ�Ϊβ�ڵ㣬��ָ���ȻΪ��
	p -> next = NULL; // ͷ��β,ͷ�ڵ���֮��ڵ����ӽض� 
	while(q){
		//��ȡ�����ڵ��ַ��׼��ѭ��
		r = q -> next;
		//�����ɷ�Ϊ2��:q->next=NULL;q->next=p;
		//�ڶ��ڵ�ָ���ÿ�,p��ֵ��ָ��ԭͷ�ڵ�(pһֱ����ͷ�ڵ�)
		//ʵ���״η�ת,��ʱqΪͷ�ڵ�
		q -> next = p;
		//���µ�ͷ�ڵ�(qֵ)������p��,qֵ�ͷų�Ϊ���ɱ���
		p = q;
		//�������ڵ��ַ(rֵ)������q��(qһֱ�����½ڵ��ַ),
		//rֵ�ͷų�Ϊ���ɱ�������õ���һ�ڵ��ַ:
		//r=q->next;ʵ��ѭ��
		q = r;
	}
	//����ת�������ͷ�ڵ㱣����head��
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


