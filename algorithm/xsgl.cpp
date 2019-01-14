#include <stdio.h> 
#include <stdlib.h> /*其它说明*/ 
#include <string.h> /*字符串函数*/ 
#include <time.h>

#define LEN sizeof(STUDENT) 
typedef struct stu /*定义结构体数组用于缓存数据*/ 
{
	char  num[6]; 
	char  name[5]; 
	int   score[3]; 
	int   sum; 
	float average; 
	int   order; 
	struct stu *next; 
}STUDENT; 

/*初始化函数*/ 
STUDENT *Init() 
{ 
	return NULL; /*返回空指针*/ 
} 

/*菜单选择函数*/ 
int Menu_Select() 
{
	int n;
	struct tm *pt; /*定义时间结构体*/ 
	time_t t;

	t=time(NULL);
	pt=localtime(&t); /*读取系统日期并把它放到结构体中*/ 
	printf("\n按任一键进入主菜单...... \n"); /*按任一键进入主菜单*/ 
	//getch(); /*从键盘读取一个字符,但不显示于屏幕*/ 
	system("pause");
	system("cls"); /*清屏*/ 
	printf("********************************************************************************\n"); 
	printf("\t\t 欢迎 Welcome to\n"); 
	printf("\n\t\t\t 使用学生管理系统1.0\n"); 
	printf("*************************************MENU***************************************\n"); 
	printf("\t\t\t1. 输入学生成绩记录 Enter the record\n"); /*输入学生成绩记录*/ 
	printf("\t\t\t2. 显示 Print the record\n"); /*显示*/ 
	printf("\t\t\t3. 寻找 Search record on name\n"); /*寻找*/ 
	printf("\t\t\t4. 删除 Delete a record\n"); /*删除*/ 
	printf("\t\t\t5. 排序 Sort to make New a file\n"); /*排序*/ 
	printf("\t\t\t6. 插入 Insert record to list\n"); /*插入*/ 
	printf("\t\t\t7. 保存 Save the file\n"); /*保存*/ 
	printf("\t\t\t8. 读取 Load the file\n"); /*读取*/ 
	printf("\t\t\t9. 退出 Quit\n"); /*退出*/  
	printf("\n********************************************************************************\n"); 
	printf("\t\t\t\t当前系统日期:%d-%d-%d\n",pt->tm_year+1900,pt->tm_mon+1,pt->tm_mday); /*显示当前系统日期*/ 
	do
	{ 
		printf("\n\t\t\t输入你的选择Enter your choice(1~9):"); 
		fflush(stdin);
		scanf("%d",&n); 
	}while(n<1||n>9); /*如果选择项不在1~9之间则重输*/ 
	return(n); /*返回选择项，主函数根据该数调用相应的函数*/ 
} 

/*输入函数*/ 
STUDENT *Create() 
{
	int i,s; 
	STUDENT *head=NULL,*p; /* 定义函数.此函数带回一个指向链表头的指针*/ 
	system("cls"); 
	for(;;) 
	{
		p=(STUDENT *)malloc(LEN); /*开辟一个新的单元*/ 
		if(!p) /*如果指针p为空*/ 
		{
			printf("\n输出内存溢出. Out of memory."); /*输出内存溢出*/ 
			return (head); /*返回头指针,下同*/ 
		} 
		printf("输入学号Enter the num(0:list end):"); 
		scanf("%s",p->num); 
		if(p->num[0]=='0') break; /*如果学号首字符为0则结束输入*/ 
		printf("输入名字Enter the name:"); 
		scanf("%s",p->name); 
		printf("请输入3门成绩Please enter the %d scores\n",3); /*提示开始输入成绩*/ 
		s=0; /*计算每个学生的总分，初值为0*/ 
		for(i=0;i<3;i++) /*3门课程循环3次*/ 
		{ 
			do
			{ 
				printf("成绩score%d:",i+1); 
				scanf("%d",&p->score[i]); 
				if(p->score[i]<0 || p->score[i]>100) /*确保成绩在0~100之间*/ 
					printf("数据错误,请重新输入 Data error,please enter again.\n"); 
			}while(p->score[i]<0 || p->score[i]>100); 
			s=s+p->score[i]; /*累加各门成绩*/ 
		} 
		p->sum=s; /*将总分保存*/ 
		p->average=(float)s/3; /*先用强制类型转换将s转换成float型,再求平均值*/ 
		p->order=0; /*未排序前此值为0*/ 
		p->next=head; /*将头结点做为新输入结点的后继结点*/ 
		head=p; /*新输入结点为新的头结点*/ 
	} 
	return(head); 
} 

/* 显示全部记录函数*/ 
void Print(STUDENT *head) 
{
	int i=0; /* 统计记录条数*/ 
	STUDENT *p; /*移动指针*/ 
	system("cls"); 
	p=head; /*初值为头指针*/ 
	printf("\n************************************STUDENT************************************\n"); 
	printf("-------------------------------------------------------------------------------\n"); 
	printf("| Rec | Num | Name | Sc1 | Sc2 | Sc3 | Sum | Ave | Order |\n"); 
	printf("-------------------------------------------------------------------------------\n"); 
	while(p!=NULL) 
	{ 
		i++; 
		printf("| %3d | %4s | %-4s | %3d | %3d | %3d | %3d | %4.2f | %-5d|\n", 
			i, p->num,p->name,p->score[0],p->score[1],p->score[2],p->sum,p->average,p->order); 
		p=p->next; 
	} 
	printf("-------------------------------------------------------------------------------\n"); 
	printf("**************************************END**************************************\n"); 
} 

/*查找记录函数*/ 
void Search(STUDENT *head) 
{
	STUDENT *p; /* 移动指针*/ 
	char s[5]; /*存放姓名用的字符数组*/ 
	system("cls"); 
	printf("请输入个姓名来查找. Please enter name for searching.\n"); 
	scanf("%s",s); 
	p=head; /*将头指针赋给p*/ 
	while(strcmp(p->name,s) && p != NULL) /*当记录的姓名不是要找的，或指针不为空时*/ 
		p=p->next; /*移动指针，指向下一结点*/ 
	if(p!=NULL) /*如果指针不为空*/ 
	{
		printf("\n*************************************FOUND************************************\n"); 
		printf("-------------------------------------------------------------------------------\n"); 
		printf("| Num | Name | sc1 | sc2 | sc3 | Sum | Ave | Order |\n"); 
		printf("-------------------------------------------------------------------------------\n"); 
		printf("| %4s | %4s | %3d | %3d | %3d | %3d | %4.2f | %-5d|\n", 
			p->num,p->name,p->score[0],p->score[1],p->score[2],p->sum,p->average,p->order); 
		printf("-------------------------------------------------------------------------------\n"); 
		printf("***************************************END**************************************\n"); 
	} 
	else 
		printf("\n没有该学生 There is no num %s student on the list.\n",s); /*显示没有该学生*/ 
} 

/*删除记录函数*/ 
STUDENT *Delete(STUDENT *head) 
{
	//int n; 
	STUDENT *p1,*p2; /*p1为查找到要删除的结点指针，p2为其前驱指针*/ 
	char c,s[6]; /*s[6]用来存放学号,c用来输入字母*/ 
	system("cls"); 
	printf("请输入要删除的学号 Please enter the Deleted num: "); 
	scanf("%s",s); 
	p1=p2=head; /*给p1和p2赋初值头指针*/ 
	while(strcmp(p1->num,s) && p1 != NULL) /*当记录的学号不是要找的，或指针不为空时*/ 
	{
		p2=p1; /*将p1指针值赋给p2作为p1的前驱指针*/ 
		p1=p1->next; /*将p1指针指向下一条记录*/ 
	} 
	if(strcmp(p1->num,s)==0) /*学号找到了*/ 
	{
		printf("**************************************FOUND************************************\n"); 
		printf("-------------------------------------------------------------------------------\n"); 
		printf("| Num | Name | sc1 | sc2 | sc3 | Sum | Ave | Order |\n"); 
		printf("-------------------------------------------------------------------------------\n"); 
		printf("| %4s | %4s | %3d | %3d | %3d | %3d | %4.2f | %-5d|\n", 
			p1->num,p1->name,p1->score[0],p1->score[1],p1->score[2],p1->sum,p1->average,p1->order); 
		printf("-------------------------------------------------------------------------------\n"); 
		printf("***************************************END**************************************\n"); 
		printf("\n是否要删除,输入Y删除,N则退出\nAre you sure to Delete the student Y/N ?"); /*提示是否要删除,输入Y删除,N则退出*/ 
		for(;;) 
		{
			scanf("%c",&c); 
			if(c=='n'||c=='N') break; /*如果不删除,则跳出本循环*/ 
			if(c=='y'||c=='Y') 
			{ 
				if(p1==head) /*若p1==head，说明被删结点是首结点*/ 
					head=p1->next; /*把第二个结点地址赋予head*/ 
				else 
					p2->next=p1->next; /*否则将一下结点地址赋给前一结点地址*/ 
				//n=n-1; 
				printf("\n学号为(Num): %s 学生以被删除(student have been Deleted.)\n",s); 
				printf("别忘了保存. Don't forget to Save.\n");break; /*删除后就跳出循环*/ 
			} 
		} 
	} 
	else 
		printf("\n没有这个学生在表上\nThere is no num %s student on the list.\n",s); /*找不到该结点*/ 
	return(head); 
} 

/*排序函数*/ 
STUDENT *Sort(STUDENT *head) 
{
	int i=0; /*保存名次*/ 
	STUDENT *p1,*p2,*t,*temp; /*定义临时指针*/ 
	temp=head->next; /*将原表的头指针所指的下一个结点作头指针*/ 
	head->next=NULL; /*第一个结点为新表的头结点*/ 
	while(temp!=NULL) /*当原表不为空时，进行排序*/ 
	{ 
		t=temp; /*取原表的头结点*/ 
		temp=temp->next; /*原表头结点指针后移*/ 
		p1=head; /*设定移动指针p1，从头指针开始*/ 
		p2=head; /*设定移动指针p2做为p1的前驱，初值为头指针*/ 
		while(t->average<p1->average&&p1!=NULL) /*作成绩平均分比较*/ 
		{ 
			p2=p1; /*待排序点值小，则新表指针后移*/ 
			p1=p1->next; 
		} 
		if(p1==p2) /*p1==p2，说明待排序点值大，应排在首位*/ 
		{ 
			t->next=p1; /*待排序点的后继为p*/ 
			head=t; /*新头结点为待排序点*/ 
		} 
		else /*待排序点应插入在中间某个位置p2和p1之间，如p为空则是尾部*/ 
		{ 
			t->next=p1; /*t的后继是p1*/ 
			p2->next=t; /*p2的后继是t*/ 
		} 
	} 
	p1=head; /*已排好序的头指针赋给p1，准备填写名次*/ 
	while(p1!=NULL) /*当p1不为空时，进行下列操作*/ 
	{ 
		i++; /*结点序号*/ 
		p1->order=i; /*将结点序号赋值给名次*/ 
		p1=p1->next; /*指针后移*/ 
	} 
	printf("排序成功 Sorting is sucessful.\n"); /*排序成功*/ 
	return (head); 
} 

/*插入记录函数*/ 
STUDENT *Insert(STUDENT *head,STUDENT *New) 
{
	STUDENT *p0,*p1,*p2; 
	//int n;
	int sum1,i; 
	p1=head; /*使p1指向第一个结点*/ 
	p0=New; /*p0指向要插入的结点*/ 
	printf("\nPlease enter a New record.\n"); /*提示输入记录信息*/ 
	printf("输入学号Enter the num:"); 
	scanf("%s",New->num); 
	printf("输入名字Enter the name:"); 
	scanf("%s",New->name); 
	printf("Please enter the %d scores.\n",3); 
	sum1=0; /*保存新记录的总分，初值为0*/ 
	for(i=0;i<3;i++) 
	{ 
		do
		{ 
			printf("成绩score%d:",i+1); 
			scanf("%d",&New->score[i]); 
			if(New->score[i]>100||New->score[i]<0) 
				printf("数据错误Data error,please enter again.\n"); 
		}while(New->score[i]>100||New->score[i]<0); 
		sum1=sum1+New->score[i]; /*累加各门成绩*/ 
	} 
	New->sum=sum1; /*将总分存入新记录中*/ 
	New->average=(float)sum1/3; 
	New->order=0; 
	if(head==NULL) /*原来的链表是空表*/ 
	{
		head=p0;
		p0->next=NULL;
	} /*使p0指向的结点作为头结点*/ 
	else 
	{
		while((p0->average<p1->average)&&(p1->next!=NULL)) 
		{
			p2=p1; /*使p2指向刚才p1指向的结点*/ 
			p1=p1->next; /*p1后移一个结点*/ 
		} 
		if(p0->average>=p1->average) 
		{
			if(head==p1)head=p0; /*插到原来第一个结点之前*/ 
			else p2->next=p0; /*插到p2指向的结点之后*/ 
			p0->next=p1;
		} 
		else 
		{
			p1->next=p0;
			p0->next=NULL;
		} /*插到最后的结点之后*/ 
	} 
	//n=n+1; /*结点数加1*/ 
	head=Sort(head); /*调用排序的函数,将学生成绩重新排序*/ 
	printf("\n学生Student %s 已被更新have been inserted.\n",New->name); 
	printf("不要忘了保存Don't forget to Save the New file.\n"); 
	return(head); 
} 

/*保存数据到文件函数*/ 
void Save(STUDENT *head) 
{
	FILE *fp; /*定义指向文件的指针*/ 
	STUDENT *p; /* 定义移动指针*/ 
	char outfile[10]; 
	printf("输出文件例如:c:\\score Enter outfile name,for example c:\\score\n"); 
	scanf("%s",outfile); 
	if((fp=fopen(outfile,"w"))==NULL) /*为输出打开一个二进制文件,为只写方式*/ 
	{ 
		printf("打不开文件Cannot open the file\n"); 
		return; /*若打不开则返回菜单*/ 
	} 
	printf("\n保存中...Saving the file......\n"); 
	p=head; /*移动指针从头指针开始*/ 
	while(p!=NULL) /*如p不为空*/ 
	{ 
		fwrite(p,LEN,1,fp); /*写入一条记录*/ 
		p=p->next; /*指针后移*/ 
	} 
	fclose(fp); /*关闭文件*/ 
	printf("保存成功....Save the file successfully!\n"); 
} 

/* 从文件读数据函数*/ 
STUDENT *Load() 
{
	STUDENT *p1,*p2,*head=NULL; /*定义记录指针变量*/ 
	FILE *fp; /* 定义指向文件的指针*/ 
	char infile[10]; 
	printf("倒入文件例如:c:\\score Enter infile name,for example c:\\score\n"); 
	scanf("%s",infile); 
	if((fp=fopen(infile,"r"))==NULL) /*打开一个二进制文件，为只读方式*/ 
	{ 
		printf("打不开文件Can not open the file.\n"); 
		return(head); 
	} 
	printf("\n寻找文件...Loading the file!\n"); 
	p1=(STUDENT *)malloc(LEN); /*开辟一个新单元*/ 
	if(!p1) 
	{ 
		printf("内存溢出!Out of memory!\n"); 
		return(head); 
	} 
	head=p1; /*申请到空间，将其作为头指针*/ 
	while(!feof(fp)) /*循环读数据直到文件尾结束*/ 
	{ 
		if(fread(p1,LEN,1,fp)!=1) break; /*如果没读到数据，跳出循环*/ 
		p1->next=(STUDENT *)malloc(LEN); /*为下一个结点开辟空间*/ 
		if(!p1->next) 
		{ 
			printf("Out of memory!\n"); 
			return (head); 
		} 
		p2=p1; /*使p2指向刚才p1指向的结点*/ 
		p1=p1->next; /*指针后移，新读入数据链到当前表尾*/ 
	} 
	p2->next=NULL; /*最后一个结点的后继指针为空*/ 
	fclose(fp); 
	printf("\n你成功的从文件中读取了数据!\nYou have success to read data from the file!\n"); 
	return (head); 
} 

/*主函数界面*/ 
int main() 
{
	STUDENT *head,New; 
	head=Init(); //链表初始化,使head的值为NULL 
	for(;;)      //循环无限次
	{
		switch(Menu_Select()) 
		{ 
		case 1:head=Create();break; 
		case 2:Print(head);break; 
		case 3:Search(head);break; 
		case 4:head=Delete(head);break; 
		case 5:head=Sort(head);break; 
		case 6:head=Insert(head,&New);break; //&New表示返回地址
		case 7:Save(head);break; 
		case 8:head=Load(); break; 
		case 9:exit(0); //如菜单返回值为9则程序结束
		} 
	} 

	return 0;
}