/*sigaction ʹ��*/
#include<stdio.h>
#include<signal.h>

void fa(int signo){
printf("�������ź�%d\n",signo);
sleep(5);//ÿ���ź��ڴ����ڼ�,Ĭ��������ͬ�ź�
printf("�źŴ������\n");
}
int main(){
	struct sigaction action={};
	action.sa_handler=fa;//�õ�һ������ָ��
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask,3);//�����ź�ʱ�����ź�3
	//action.sa_flags=SA_ONESHOT;//SA_RESETHAND
	//SA_ONESHOTִ��һ�δ������ָ�ΪĬ�ϴ���
	action.sa_flags=SA_NOMASK;//�����ڼ�,��������ͬ�ź�
	sigaction(SIGINT,&action,NULL);
	while(1);
}
