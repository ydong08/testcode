//Download by http://www.NewXing.com
// ECDSA.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ECDSA.h"

#include "MainFrm.h"
#include "ECDSADoc.h"
#include "ECDSAView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECDSAApp

BEGIN_MESSAGE_MAP(CECDSAApp, CWinApp)
	//{{AFX_MSG_MAP(CECDSAApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECDSAApp construction

CECDSAApp::CECDSAApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CECDSAApp object

CECDSAApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CECDSAApp initialization

BOOL CECDSAApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CECDSADoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CECDSAView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CECDSAApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

//BigInteger类的实现，自己定义的
#include "iostream.h"
#include "stdio.h"
#include "windows.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"

static seed=0;

BigInteger::BigInteger()        //构造函数,将每个节点置空
{
	Head=End=TempNode=NULL;
}

BigInteger::BigInteger(char i)  //构造函数,只拥有一位的大整数
{
	Head=End=TempNode=NULL;
	TempNode=new Node;
	TempNode->Num=i;
	TempNode->Prev=NULL;
	Head=End=TempNode;
	TempNode->Next=NULL;	
}

BigInteger::BigInteger(const BigInteger &BigNum)     //拷贝构造
{
	Node *p;
	Head=End=TempNode=NULL;
	p=BigNum.Head;
	while(p)
	{
		AddEnd(p->Num);
		p=p->Next;
	}
}

BigInteger::~BigInteger()        //析构
{
	Node *NextNode;
	if(Head==NULL)
		return;
	TempNode=Head;
	while(TempNode)
	{
		NextNode=TempNode->Next;
		delete TempNode;
		TempNode=NextNode;
	}
	Head=NULL;
	End=NULL;
	TempNode=NULL;
}

void BigInteger::AddHead(char Num)        //在链表头插入节点的操作
{
	TempNode=new Node;
	TempNode->Num=Num;
	TempNode->Prev=NULL;
	if(!Head)
	{
		Head=End=TempNode;
		TempNode->Next=NULL;
	}
	else
	{
		TempNode->Next=Head;
		Head->Prev=TempNode;
		Head=TempNode;
	}
}

void BigInteger::AddEnd(char Num)       //在链表尾插入节点的操作
{
	TempNode=new Node;
	TempNode->Num=Num;
	TempNode->Next=NULL;
	if(!Head)
	{
		Head=End=TempNode;
		TempNode->Prev=NULL;
	}
	else
	{
		TempNode->Prev=End;
		End->Next=TempNode;
		End=TempNode;
	}
}


//调整大整数,如果最前面是0的话则表明该数就是0
void AdjustBigNum(BigInteger &BigNum)
{
	Node *temp;
	temp=BigNum.Head;
	while (temp&&int(temp->Num)==0&&temp!=BigNum.End) 
	{
			BigNum.Head=temp->Next;
			delete temp;
			temp=BigNum.Head;
	}	
}
//将大整数转化为二进制数,并不影响*this的值
void BigInteger::BigNumToIndex(int b[],int &count)
{
	BigInteger BigNum=*this,temp2(2),temp;
	count=0;
	while (int(BigNum.Head->Num))
	{
		temp=BigNum%temp2;
		b[count++]=int(temp.Head->Num);
		BigNum=BigNum/temp2;
	}
}
//正数
BigInteger BigInteger::operator ++()//重载++
{
	BigInteger temp(1);
	*this=*this+temp;
	AdjustBigNum(*this);
	return *this;
}
//正数
BigInteger BigInteger::operator --()//重载--
{
	int i=1;
	BigInteger temp;
	temp.AddHead(char(i));
	*this=*this-temp;
	AdjustBigNum(*this);
	return *this;
}

//大整数加运算，只能是两正整数，需要处理负数时转化为减法处理
BigInteger BigInteger::operator + (BigInteger BigNum2)//重载"+"
{ 
	BigInteger result;
	Node *temp1,*temp2;
	int TempNum,rest=0;
	temp1=this->End;//将临时链表首地址放置到输入链表的尾部
	temp2=BigNum2.End;
	while(temp1 && temp2)
	{
		TempNum=int(temp1->Num)+int(temp2->Num)+rest;//节点内元素相加并加上进位rest
		if(TempNum>9)//判断相加结果是否会产生进位.
		{
			TempNum=TempNum-10;
			rest=1;
		}
		else
			rest=0;
		result.AddHead(char(TempNum));//将结果放置到最终结果链表里
		temp1=temp1->Prev;//将两个相加的链表前移一位
		temp2=temp2->Prev;
	}
	if(temp2)
		temp1=temp2;//处理两链表中剩下的部分
	while(temp1)
	{
		int(TempNum)=int(temp1->Num)+rest;//节点内元素加上进位rest
		if(TempNum>9)
		{
			TempNum=TempNum-10;
			rest=1;
		}
		else
			rest=0;
		result.AddHead(char(TempNum));//将结果放置到最终结果链表里
		temp1=temp1->Prev;
	}
	if(rest)
		result.AddHead(char(rest));//考虑最后的进位是否存在,如果存在则存入链表的首部.
	AdjustBigNum(result);
	return result;
}

//a-b,要求a>=b,小-大时调成大-小再加负号,a、b中有负的要转为正的处理
BigInteger BigInteger::operator - (BigInteger BigNum2)//重载"-"
{
	BigInteger BigNum1=*this,temp,result;
	Node *temp1,*temp2;
	bool flag;
	int TempNum,rest=0;
	if (int(BigNum1.Head->Num)<0&&int(BigNum2.Head->Num)<0)
	{
		BigNum2.Head->Num=char(int(BigNum2.Head->Num)*(-1));//转正
		BigNum1.Head->Num=char(int(BigNum1.Head->Num)*(-1));//转正
		temp=BigNum1;
		BigNum1=BigNum2;
		BigNum2=temp;
	}
	if (int(BigNum1.Head->Num)<0&&int(BigNum2.Head->Num)>0) 
	{
		BigNum1.Head->Num=char(int(BigNum1.Head->Num)*(-1));//转正
		result=BigNum2+BigNum1;
		result.Head->Num=char(int(result.Head->Num)*(-1));//变负
		return result;
	}
	if (int(BigNum1.Head->Num)>0&&int(BigNum2.Head->Num)<0) 
	{
		BigNum2.Head->Num=char(int(BigNum2.Head->Num)*(-1));//转正
		result=BigNum2+BigNum1;
		return result;
	}
	
	if (Compare(BigNum1,BigNum2)==1) 
	{
		temp1=BigNum1.End;//将临时链表首地址放置到输入链表的尾部
		temp2=BigNum2.End;
		flag=true;
	}
	else
	{
		temp1=BigNum2.End;//将临时链表首地址放置到输入链表的尾部
		temp2=BigNum1.End;
		flag=false;
	}
	
	while(temp1 && temp2)
	{

		TempNum=int(temp1->Num)-int(temp2->Num)+rest;//节点内元素相加并加上借位rest
		if(TempNum<0)//判断相加结果是否会产生借位.
		{
			TempNum=10+TempNum;
			rest=-1;
		}
		else
			rest=0;
		result.AddHead(char(TempNum));//将结果放置到最终结果链表里
		temp1=temp1->Prev;//将两个相减的链表前移一位
		temp2=temp2->Prev;
	}
	//处理两链表中剩下的部分	
	while(temp1)
	{
		int(TempNum)=int(temp1->Num)+rest;//节点内元素加上借位rest
		if(TempNum<0)
		{
			TempNum=10+TempNum;
			rest=-1;
		}
		else
			rest=0;
		result.AddHead(char(TempNum));//将结果放置到最终结果链表里
		temp1=temp1->Prev;
	}
	if(rest)
	{
		result.Head->Num-=10;
	}
	AdjustBigNum(result);
	if (!flag) 
	{
		result.Head->Num=char(int(result.Head->Num)*(-1));
	}
	return result;
}


BigInteger BigInteger::operator * (BigInteger BigNum2)//对*进行重载
{
	BigInteger BigNum1=*this,temp,result;
	Node *temp1,*temp2,*tempa,*tempb;
	bool flag=true;
	int TempNum,rest,i=0,rest2;
	int k;

	if (int(BigNum1.Head->Num)*int(BigNum2.Head->Num)<0) 
	{
		flag=false;
	}
	if (int(BigNum1.Head->Num)<0)
	{
		BigNum1.Head->Num=char(int(BigNum1.Head->Num)*(-1));
	}
	if (int(BigNum2.Head->Num)<0)
	{
		BigNum2.Head->Num=char(int(BigNum2.Head->Num)*(-1));
	}

	temp1=BigNum1.End;
	temp2=BigNum2.End;
	while(temp2)//由乘数的存在与否判断是否去乘被乘数的每个位
	{
		rest=0;
		//对乘数中的每一位都逐一去乘被乘数
		while(temp1!=NULL)
		{
			TempNum=int(temp1->Num)*int(temp2->Num)+rest;
			if(TempNum>9)
			{ 
				rest=TempNum/10; //进位由相乘结果与10做商求得
				TempNum=TempNum%10; //由相乘结果与10求模取个位
			}
			else
				rest=0;
			temp.AddHead(char(TempNum));//存入临时链表
			temp1=temp1->Prev;
		}
		if(rest!=0)
			temp.AddHead(char(rest));
		for(k=i;k>=1;k--)
			temp.AddEnd(char(0));//判断应该在链表后面补几个0
		i++; //每次乘完后计数,用来下一次的补0
		temp1=BigNum1.End;//把被乘数重新置到尾,用来让乘数下一次去乘每个元素
		temp2=temp2->Prev;//将乘数取出链表的前驱
		tempa=result.End;//下面进行的是将每次乘数与被乘数的相乘结果累加放到最终链表里等待输出
		if(result.Head!=NULL)//下面过程与"+"重载基本一样,只是多了对临时链表的置空.
		{
			result.End=temp.Head;
			result.Head=NULL;
		}
		tempb=temp.End;
		rest2=0;
		while(tempa!=NULL && tempb!=NULL)
		{
			TempNum=int(tempa->Num)+int(tempb->Num)+rest2;
			if(TempNum>9)
			{
				TempNum=TempNum-10;
				rest2=1;
			}
			else
				rest2=0;
			result.AddHead(char(TempNum));
			tempa=tempa->Prev;
			tempb=tempb->Prev;
		}
		if(tempb)
			tempa=tempb;
		while(tempa)
		{
			int(TempNum)=int(tempa->Num)+rest2;
			if(TempNum>9)
			{
				TempNum=TempNum-10;
				rest2=1;
			}
			else
				rest2=0;
			result.AddHead(char(TempNum));
			tempa=tempa->Prev;
		}
		if(rest2)
			result.AddHead(char(rest2));
		if(temp.Head!=NULL)
		{
			temp.End=temp.Head;
			temp.Head=NULL;
		}
		tempb=NULL;
	}
	AdjustBigNum(result);
	if (!flag)
	{
		result.Head->Num=char(int(result.Head->Num)*(-1));
	}
	
	return result;
}
/************************************************************************/
/* 将除法转化为减法来处理,基本思想如下:
						div=0;
						while(a>=0)
						{
							a-=b;
							div++;
						}
						div--;	
这种方式直接处理效率很低,后来改进为得到商的各位数字,将除数扩大处理,
速度提高了很多,现在可以对整数的整除均可以进行
																		*/
/************************************************************************/
BigInteger BigInteger::operator / (BigInteger BigNum2)//对/号进行重载
{
	BigInteger BigNum1,result(0);
	bool flag=true;
	BigNum1=*this;
	if (int(BigNum1.Head->Num)*int(BigNum2.Head->Num)<0) 
	{
		flag=false;
	}
	if (int(BigNum1.Head->Num)<0)
	{
		BigNum1.Head->Num=char(int(BigNum1.Head->Num)*(-1));
	}
	if (int(BigNum2.Head->Num)<0)
	{
		BigNum2.Head->Num=char(int(BigNum2.Head->Num)*(-1));
	}

	int cmp=Compare(BigNum1,BigNum2);
	if (cmp==-1)
	{
		result.AddEnd(0);
	}
	else if (cmp==0)
	{
		result.AddEnd(1);
	}
	else
	{
		int n,t,i=0,k,sub;//BigNum1,BigNum2的长度
		Node *p1=BigNum1.Head;
		Node *p2=BigNum2.Head;
		while (p1)
		{
			n++;
			p1=p1->Next;
		}
		while (p2)
		{
			t++;
			p2=p2->Next;
		}
		sub=n-t;
		BigInteger Bm1,Bm2=BigNum2;
		BigInteger temp0(0),temp1(1),expand,temp2,temp3,temp;
		/************************************************************************/
		/* 对此情形的整除采用每步获取商的各位数字的形式得到,余数小于0时退出    */
		/************************************************************************/
		while((!(Compare(BigNum1,Bm2)==-1)))
		{
			Bm1=BigNum1;//保存每步的被除数
			expand=temp1;
			n=t=0;
			p1=BigNum1.Head;
			p2=BigNum2.Head;
			while (p1)
			{
				n++;
				p1=p1->Next;
			}
			while (p2)
			{
				t++;
				p2=p2->Next;
			}
			temp2=Transform(10); 
			temp3=Transform(n-t-1);
			//先将除数扩大10^(n-t-1)倍,商再扩大这么多倍即可得到除数的最高位后添0的数值
			if (n-t-1>0) 
			{ 
				expand=temp2^temp3;
				BigNum2=BigNum2*expand;
			}
			k=0;
			while(int(BigNum1.Head->Num)>=0)
			{
				BigNum1=BigNum1-BigNum2;
				k++;
			}
			k--;
			temp=expand*Transform(k);
			result=result+temp;
			BigNum1=Bm1-BigNum2*Transform(k);//每步的余数代替被除数
			BigNum2=Bm2;//恢复除数
		}
	}
	AdjustBigNum(result);	
	if (!flag)
	{
		result.Head->Num=char(int(result.Head->Num)*(-1));
	}
	return result;	
}

BigInteger BigInteger::operator % (BigInteger BigNum2)//对%号进行重载
{
	BigInteger BigNum1,temp,temp1(-1),result;
	BigNum1=*this;
	//为负则化为正处理,'+'只能对两个正整数有效
	while (int(BigNum1.Head->Num)<0)
	{
		BigNum1=BigNum1*temp1;//转正
		BigNum1=BigNum2-BigNum1;
	}	
	temp=BigNum1/BigNum2;
	result=BigNum1-(temp*BigNum2);
	AdjustBigNum(result);
	return result;	
}
/************************************************************************/
/*底数和指数都是正数,可以按模重复平方法的思路计算,只是计算过程中不用取余*/
/************************************************************************/
BigInteger BigInteger::operator ^ (BigInteger BigNum2)
{
	int count=0;
	BigInteger result(1),temp,temp2(2);
	BigInteger &BigNum1=*this;
	int b[200];
	//将BigNum2转换为二进制
	while(int(BigNum2.Head->Num))
	{
		temp=BigNum2%temp2;
		b[count++]=int(temp.Head->Num);
		BigNum2=BigNum2/temp2;
	}
	//按模重复平方法计算
	for(int i=count-1;i>=0;i--)
	{
		result=result*result;
		if(b[i])
			result=result*BigNum1;
	}
	return result;
}

BigInteger BigInteger::operator = (BigInteger BigNum)//对=号进行重载
{
	if(this==&BigNum)
		return *this;
	this->~BigInteger();
	Node *p;
	TempNode=Head=End=NULL;
	p=BigNum.Head;
	while(p)
	{
		AddEnd(p->Num);
		p=p->Next;
	}
	return *this;
}

CString disp(BigInteger BigNum)//输出链表
{
	CString str;
	char s[2]={'\0','\0'};
	Node *p=BigNum.Head;
	while(p)
	{
		str+=itoa(int(p->Num),s,10);
		p=p->Next;
	}
	str+="\r\n";
	return str;
}
//a mod n 的逆,存在则返回逆，否则返回0
BigInteger Inv(BigInteger a,BigInteger n)
{
	BigInteger b1(0),b2(1),d1(n),d2(a%n),temp,temp0(0);
	if (int(a.Head->Num)<0)
	{
		a=a+n;
	}
	while (int(d2.Head->Num))
	{
		temp=b2;
		b2=b1-(b2*(d1/d2));	 
		b1=temp;
		temp=d2;
		d2=d1-((d1/d2)*d2);
		d1=temp;
	}
	while (int(b1.Head->Num)<0)
	{
		b1.Head->Num=char(int(b1.Head->Num)*(-1));
		b1=n-b1;
	}
	if (d1.Head==d1.End&&int(d1.Head->Num==1))
	{
		return b1%n;
	}
	else
		return temp0;
	
}
//大整数的比较:a>b则返回1,相等返回0,a<b返回-1,只针对非负整数
int Compare(BigInteger BigNum1,BigInteger BigNum2)
{
	int result=0;//初始化为相等
	Node *temp1,*temp2;
	temp1=BigNum1.End;
	temp2=BigNum2.End;
	while (temp1&&temp2)
	{
		if (int(temp1->Num)>int(temp2->Num))
		{
			result=1;
		}
		if (int(temp1->Num)<int(temp2->Num))
		{
			result=-1;
		}
		temp1=temp1->Prev;
		temp2=temp2->Prev; 
	}
	if (temp1)
	{
		result=1;
	}
	if (temp2)
	{
		result=-1;
	}
	return result;
}
//如果BigNum==0返回1，否则返回0
int Compare(BigInteger BigNum)
{
	if (int(BigNum.Head->Num))
	{
		return 0;
	}
	else
		return 1;
}
//随机产生一个n位的大整数
BigInteger Rand(int n)
{
	BigInteger result,temp1(1);
	int number;
	srand(unsigned(GetTickCount()+seed));	
	number=rand();//随机产生种子
	seed=number;
	srand(number);//用种子初始化随机发生器
	number=1+rand()%9;//产生1-9的数字作为第一位
	result.AddEnd(char(number));
	n--;
	while (n)
	{
		number=rand();
		srand(number+seed);
		number=rand()%10;
		result.AddEnd(char(number));
		n--;
	}
	seed=seed/2;
	//调整使最后一位为奇数，利于后面产生素数
	if(int(result.End->Num)%2==0)
		result=result+temp1;

	return result;
}
/************************************************************************/
/*     随机产生大整数Bignum以内的大整数,即0--BigNum-1之间
		方法: 随机生成一个相同位数的大整数,再求模                       */
/************************************************************************/
BigInteger Rand(BigInteger BigNum)
{
	Node *p=BigNum.Head;
	BigInteger result;
	int length=0;//BigNum的位数
	int num;
	while (p)
	{
		length++;
		p=p->Next;
	}
	srand(unsigned(GetTickCount()));	
	randmize=rand()%1500;//随机产生种子
	srand(randmize);
	for(int i=1;i<=length;i++)
	{
		srand(randmize);//用种子初始化随机发生器
		randmize+=rand()%197;//保证种子不同
		num=rand()%10;
		result.AddEnd(num);
	}
	AdjustBigNum(result);
	result=result%BigNum;
	return result;
}
//将long型变量转化为大整数BigInteger型
BigInteger Transform(long n)
{
	BigInteger result;
	int temp;
	bool flag;
	if (n<0)
	{
		flag=false;
		n=-n;
	}
	while(n)
	{
		temp=n%10;//从个位开始依次取各位的数字
		result.AddHead(char(temp));
		n=n/10;
	}
	if (!flag) 
	{
		result.Head->Num=char(int(result.Head->Num)*(-1));//n为负数则转化为负的大整数
	}
	return result;
}


//大整数a^m%n运算,用模重复平方法求解
BigInteger Calculate_exp(BigInteger a,BigInteger m,BigInteger n)
{
	int i;
	int count=0;
	BigInteger d(1),temp,temp2(2);
	int b[400];
	//计算m转换为二进制
	while(int(m.Head->Num))
	{
		temp=m%temp2;
		b[count++]=int(temp.Head->Num);
		m=m/temp2;
	}
	//按模重复平方法计算
	for(i=count-1;i>=0;i--)
	{
		d=d*d%n;
		if(b[i])
			d=d*(a%n)%n;
	}
	return d;
}
/************************************************************************/
/*素性检验之前的筛选,即除以100以内的小素数								*/
/************************************************************************/
bool Select(BigInteger BigNum,int n)
{
	int a[]={3,5,7,11,13,17,19,23,29,31,34,37,41,43,47,51,57,59,61,67,71,73,79,83,89,91,97};
	if (int(BigNum.End->Num)%2==0) 
	{
		return false;
	}
	int i=0;
	BigInteger temp;
	while (i<27&&i<n) 
	{
		temp=Transform(a[i++]);
		if (Compare(BigNum%temp)) 
		{
			return false;
		}
	}
	return true;
}
/************************************************************************/
/* Rabin_Miller素性检验													*/
/************************************************************************/
bool M_Rabin(BigInteger n,int k)
{
	BigInteger t,b,r;
	long s=0,i=3;
	BigInteger nn,temp,temp0(0),temp1(1),temp2(2);
	nn=n-temp1;
	t=n-temp1;
	if (Compare(n,temp2)==0||Compare(n,temp1+temp2)==0) 
	{
		return true;
	}
	if (Compare(n,temp1)==0||Compare(n,temp2+temp2)==0)
	{
		return false;
	}
	temp=nn%temp2;
	while (Compare(temp))
	{
		t=nn/temp2;
		nn=nn/temp2;
		s++;
		temp=nn%temp2;
	}
	nn=n-temp1;
	while (k)
	{
		b=temp2+Rand(n-temp2-temp2);//随机选取2<=b<=n-2
		r=Calculate_exp(b,t,n);
		if (Compare(r,temp1)&&Compare(r,nn))
		{
			r=Calculate_exp(r,temp2,n);
			i++;
			while (Compare(r,nn)&&i<s+2) 
			{
				r=Calculate_exp(r,temp2,n);
				i++;
			}
			if (Compare(r,nn))
			{
				return false;
			}
		}
		i=3;
		k--;
	}
	return true;
}
//生成m位的伪随机大素数
BigInteger Produce_Prime(int m)
{
	BigInteger result=Rand(m),temp2(2);
	while (1)
	{
		if (!Select(result,2*m)) 
		{
			result=result+temp2;
		}
		else
		{
			if (!M_Rabin(result,5))
			{
				result=result+temp2;
			}
			else
				return result;
		}
	}	
}
//将一串数字转化为大整数
BigInteger CStringToBignum(CString sub)
{
	int length=sub.GetLength();
	BigInteger result;
	TCHAR c;
	int num;
	for(int i=0;i<length;i++)
	{
		c=sub[i];
		num=c-'0';
		result.AddEnd(num);
	}
	return result;
}
/////////////////////////////////////////////////////////////////////////////
// CECDSAApp message handlers

