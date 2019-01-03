/*链表逆序*/

void reverse(test* head){
	test* pe = head;
	test* ps = head->next;
	while(ps)	{ 
		/*利用头节点最终成为尾节点且尾节点的next指针为空的特点
		   将每次循环将要操作的下一个节点保存在next指针中*/
		pe->next = ps->next;
		ps->next = head;
		head = ps;
		ps = pe->next;
	}
}
