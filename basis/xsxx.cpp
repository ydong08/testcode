#include<stdio.h>  /*I/O����*/
#include<stdlib.h> /*����˵��*/
#include<string.h> /*�ַ�������*/
#define BUFLEN 100 /* ����������ַ��� */
#define LEN 15 /* ѧ�ź���������ַ���,ʵ������� */
#define N 100 /* ���ѧ������,ʵ�������*/
struct record /*�ṹ��*/
{
char code[LEN+1]; /* ѧ�� */
char name[LEN+1]; /* ���� */
int age; /* ���� */
char sex[3]; /* �Ա� */
char time[LEN+1]; /* �������� */
char add[30]; /* ��ͥ��ַ */
char tel[LEN+1]; /* �绰���� */
char mail[30]; /* �����ʼ���ַ */
}stu[N];
int k=1,n,m; /* ����ȫ�ֱ��� */ 
void readfile();/* �������� */
void seek();
void modify();
void insert();
void del();
void display();
void save();
void menu(); 
int main()
{  
while(k)
menu();
system("pause");
return 0;
} 
void help()
{ 
printf("\n0.��ӭʹ��ϵͳ������\n");
printf("\n1.����ϵͳ��,��ˢ��ѧ����Ϣ,�ٲ�ѯ;\n");
printf("\n2.���ղ˵���ʾ�������ִ���;\n");
printf("\n3.����ѧ����Ϣ��,�мǱ��水7;\n");
printf("\n4.лл����ʹ�ã�\n");
} 
void readfile()/* ������Ϣ */
{
char *p="student.txt";
FILE *fp;
int i=0;
if ((fp=fopen("student.txt","r"))==NULL)
{  
printf("Open file %s error! Strike any key to exit!",p);
system("pause");
exit(0);
}
while(fscanf(fp,"%s %s%d%s %s %s %s %s",stu[i].code,stu[i].name,&stu[i].age,
stu[i].sex,stu[i].time,stu[i].add,stu[i].tel,stu[i].mail)==8)
{
i++;
i=i;
}
fclose(fp);
n=i;
printf("¼����ϣ�\n");
} 

void seek() /*����*/
{
int i,item,flag;
char s1[21]; /* ��������ѧ�������+1Ϊ׼ */
printf("------------------\n");
printf("-----1.��ѧ�Ų�ѯ-----\n");
printf("-----2.��������ѯ-----\n");
printf("-----3.�˳����˵�-----\n");
printf("------------------\n");
while(1)
{
printf("��ѡ���Ӳ˵����:");
scanf("%d",&item);
flag=0;
switch(item)
{
case 1:
printf("������Ҫ��ѯ��ѧ����ѧ��:\n");
scanf("%s",s1);
for(i=0;i<n;i++)
if(strcmp(stu[i].code,s1)==0)
{
flag=1;
printf("ѧ��ѧ��  ѧ������  ����  �Ա�   ��������   ��ַ     �绰       E-mail\n");
printf("--------------------------------------------------------------------\n");
printf("%6s %7s %6d %5s %9s %8s %10s %14s\n",stu[i].code,stu[i].name,stu[i].age,
stu[i].sex,stu[i].time,stu[i].add,stu[i].tel,stu[i].mail);
}
if(flag==0)
printf("��ѧ�Ų����ڣ�\n"); break;
case 2:
printf("������Ҫ��ѯ��ѧ��������:\n");
scanf("%s",s1);
for(i=0;i<n;i++)
if(strcmp(stu[i].name,s1)==0)
{
flag=1;
printf("ѧ��ѧ��  ѧ������  ����  �Ա�   ��������   ��ַ     �绰     E-mail\n");
printf("--------------------------------------------------------------------\n");
printf("%6s %7s %6d %5s %9s %8s %10s %14s\n",stu[i].code,stu[i].name,stu[i].age,
stu[i].sex,stu[i].time,stu[i].add,stu[i].tel,stu[i].mail);
}
if(flag==0)
printf("�����������ڣ�\n"); break;
case 3:return;
default:printf("����1-3֮��ѡ��\n");
}
}
} 
void modify() /*�޸���Ϣ*/
{
int i,item,num;
char sex1[3],s1[LEN+1],s2[LEN+1]; /* ��������ѧ�������+1Ϊ׼ */
printf("������ҪҪ�޸ĵ�ѧ����ѧ��:\n");
scanf("%s",s1);
for(i=0;i<n;i++)
if(strcmp(stu[i].code,s1)==0) /*�Ƚ��ַ����Ƿ����*/
num=i;
printf("------------------\n");
printf("1.�޸�����\n");
printf("2.�޸�����\n");
printf("3.�޸��Ա�\n");
printf("4.�޸ĳ�������\n");
printf("5.�޸ĵ�ַ\n");
printf("6.�޸ĵ绰����\n");
printf("7.�޸�E-mail��ַ\n");
printf("8.�˳����˵�\n");
printf("------------------\n");
while(1)
{
printf("��ѡ���Ӳ˵����:");
scanf("%d",&item);
switch(item)
{
case 1:
printf("�������µ�����:\n");
scanf("%s",s2);
strcpy(stu[num].name,s2); break;
case 2:
printf("�������µ�����:\n");
scanf("%d",&stu[num].age);break;
case 3:
printf("�������µ��Ա�:\n");
scanf("%s",sex1);
strcpy(stu[num].sex,sex1); break;
case 4:
printf("�������µĳ�������:\n");
scanf("%s",s2);
strcpy(stu[num].time,s2); break;
case 5:
printf("�������µĵ�ַ:\n");
scanf("%s",s2);
strcpy(stu[num].add,s2); break;
case 6:
printf("�������µĵ绰����:\n");
scanf("%s",s2);
strcpy(stu[num].tel,s2); break;
case 7:
printf("�������µ�E-mail��ַ:\n");
scanf("%s",s2);
strcpy(stu[num].mail,s2); break;
case 8:return;
default:printf("����1-8֮��ѡ��\n");
}
}
} 
 void sort()/*��ѧ������*/
{
int i,j,*p,*q,s;
  char temp[10];
for(i=0;i<n-1;i++)
{
for(j=n-1;j>i;j--)
if(strcmp(stu[j-1].code,stu[j].code)>0)
{
strcpy(temp,stu[j-1].code);
strcpy(stu[j-1].code,stu[j].code);
strcpy(stu[j].code,temp);
strcpy(temp,stu[j-1].name);
strcpy(stu[j-1].name,stu[j].name);
strcpy(stu[j].name,temp);
strcpy(temp,stu[j-1].sex);
strcpy(stu[j-1].sex,stu[j].sex);
strcpy(stu[j].sex,temp);
strcpy(temp,stu[j-1].time);
strcpy(stu[j-1].time,stu[j].time);
strcpy(stu[j].time,temp);
strcpy(temp,stu[j-1].add);
strcpy(stu[j-1].add,stu[j].add);
strcpy(stu[j].add,temp);
strcpy(temp,stu[j-1].tel);
strcpy(stu[j-1].tel,stu[j].tel);
strcpy(stu[j].tel,temp);
strcpy(temp,stu[j-1].mail);
strcpy(stu[j-1].mail,stu[j].mail);
strcpy(stu[j].mail,temp);
 p=&stu[j-1].age;
 q=&stu[j].age;
 s=*q;
 *q=*p;
 *p=s;
}
}
} 
void insert() /*���뺯��*/
{
 int i=n,j,flag;
printf("����������ӵ�ѧ����:\n");
scanf("%d",&m);
do
{
 flag=1;
while(flag)
{
flag=0;
printf("������� %d ��ѧ����ѧ��:\n",i+1);
scanf("%s",stu[i].code);
for(j=0;j<i;j++)
if(strcmp(stu[i].code,stu[j].code)==0)
{
printf("���и�ѧ��,���������¼��!\n");
flag=1;
break; /*�����ظ������˳��ò�ѭ��,����ж��ٶ�*/
}
}
printf("������� %d ��ѧ��������:\n",i+1);
scanf("%s",stu[i].name);
printf("������� %d ��ѧ��������:\n",i+1);
scanf("%d",&stu[i].age);
printf("������� %d ��ѧ�����Ա�:\n",i+1);
scanf("%s",stu[i].sex);
printf("������� %d ��ѧ���ĳ�������:(��ʽ:��.��)\n",i+1);
scanf("%s",stu[i].time);
printf("������� %d ��ѧ���ĵ�ַ:\n",i+1);
scanf("%s",stu[i].add);
printf("������� %d ��ѧ���ĵ绰:\n",i+1);
scanf("%s",stu[i].tel);
printf("������� %d ��ѧ����E-mail:\n",i+1);
scanf("%s",stu[i].mail);
if(flag==0)
{
 i=i;
i++;
}
}
while(i<n+m);
n+=m;
printf("¼����ϣ�\n\n");
sort();
} 
void del()
{
int i,j,flag=0;
char s1[LEN+1];
printf("������Ҫɾ��ѧ����ѧ��:\n");
scanf("%s",s1);
for(i=0;i<n;i++)
if(strcmp(stu[i].code,s1)==0)
{
 flag=1;
for(j=i;j<n-1;j++)
stu[j]=stu[j+1];
}
if(flag==0)
printf("��ѧ�Ų����ڣ�\n");
if(flag==1)
{
printf("ɾ���ɹ�,��ʾ�����ѡ��˵�6\n");
n--;
}
} 
void display()
{
 int i;
printf("����ѧ������ϢΪ:\n");
printf("ѧ��ѧ��  ѧ������  ����  �Ա�   ��������   ��ַ     �绰     E-mail\n");
printf("--------------------------------------------------------------------\n");
for(i=0;i<n;i++)
{ 
printf("%6s %7s %5d %5s %9s %8s %10s %14s\n",stu[i].code,stu[i].name,stu[i].age,
stu[i].sex,stu[i].time,stu[i].add,stu[i].tel,stu[i].mail);
}
} 
void save()
{
int i;
FILE *fp;
fp=fopen("student.txt","w"); /*д��*/
for(i=0;i<n;i++)
{
fprintf(fp,"%s %s %d %s %s %s %s %s\n",stu[i].code,stu[i].name,stu[i].age,
stu[i].sex,stu[i].time,stu[i].add,stu[i].tel,stu[i].mail);
}
fclose(fp);
} 
void menu()/* ���� */
{
int num;
printf("*********************ϵͳ���ܲ˵�************************       \n");
printf("��������:��ѯǰ����ˢ��ϵͳ��    \n");
printf("     ----------------------   ----------------------   \n");
printf("     *********************************************     \n");
printf("     * 0.ϵͳ������˵��   * *  1.ˢ��ѧ����Ϣ    *     \n");
printf("     *********************************************     \n"); 
printf("     * 2.��ѯѧ����Ϣ     * *  3.�޸�ѧ����Ϣ    *     \n");
printf("     *********************************************     \n");
printf("     * 4.����ѧ����Ϣ     * *  5.��ѧ��ɾ����Ϣ  *     \n");
printf("     *********************************************     \n");
printf("     * 6.��ʾ��ǰ��Ϣ     * *  7.���浱ǰѧ����Ϣ*     \n");
printf("     ********************** **********************     \n");
printf("     * 8.�˳�ϵͳ         *                            \n");
printf("     **********************                            \n");
printf("     ----------------------   ----------------------                           \n");
printf("��ѡ��˵����:");
scanf("%d",&num);
switch(num)
{ 
case 0:help();break;
case 1:readfile();break;
case 2:seek();break;
case 3:modify();break;
case 4:insert();break;
case 5:del();break;
case 6:display();break;
case 7:save();break;
case 8:k=0;break;
default:printf("����0-8֮��ѡ��\n");
}