/*�̶��������л�*/
/*������ָ������˵�������ָ��1ÿ���ƶ�һ����ָ��2ÿ���ƶ�����
   ָ��1��ָ��2���ʱ����ʾ�л�*/
int  testListRing(Node* head) {
	Node* t1=head; *t2=head;
	while(t1->next && t2->next) {
		t1=t1->next;
		if(NULL==(t2=t2->next->next)
			return 0;//���л���t2!=NULL
		if(t1=t2)	
			return 1;
	}
	return 0;
}
