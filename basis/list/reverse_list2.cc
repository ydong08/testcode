/*��������*/

void reverse(test* head){
	test* pe = head;
	test* ps = head->next;
	while(ps)	{ 
		/*����ͷ�ڵ����ճ�Ϊβ�ڵ���β�ڵ��nextָ��Ϊ�յ��ص�
		   ��ÿ��ѭ����Ҫ��������һ���ڵ㱣����nextָ����*/
		pe->next = ps->next;
		ps->next = head;
		head = ps;
		ps = pe->next;
	}
}
