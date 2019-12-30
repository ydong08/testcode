/*盘断链表中有环*/
/*用两个指针遍历此单向链表，指针1每次移动一步，指针2每次移动两步
   指针1与指针2相等时，表示有环*/
int  testListRing(Node* head) {
	Node* t1=head; *t2=head;
	while(t1->next && t2->next) {
		t1=t1->next;
		if(NULL==(t2=t2->next->next)
			return 0;//若有环则t2!=NULL
		if(t1=t2)	
			return 1;
	}
	return 0;
}
