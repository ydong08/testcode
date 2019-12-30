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

//BigInteger���ʵ�֣��Լ������
#include "iostream.h"
#include "stdio.h"
#include "windows.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"

static seed=0;

BigInteger::BigInteger()        //���캯��,��ÿ���ڵ��ÿ�
{
	Head=End=TempNode=NULL;
}

BigInteger::BigInteger(char i)  //���캯��,ֻӵ��һλ�Ĵ�����
{
	Head=End=TempNode=NULL;
	TempNode=new Node;
	TempNode->Num=i;
	TempNode->Prev=NULL;
	Head=End=TempNode;
	TempNode->Next=NULL;	
}

BigInteger::BigInteger(const BigInteger &BigNum)     //��������
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

BigInteger::~BigInteger()        //����
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

void BigInteger::AddHead(char Num)        //������ͷ����ڵ�Ĳ���
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

void BigInteger::AddEnd(char Num)       //������β����ڵ�Ĳ���
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


//����������,�����ǰ����0�Ļ��������������0
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
//��������ת��Ϊ��������,����Ӱ��*this��ֵ
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
//����
BigInteger BigInteger::operator ++()//����++
{
	BigInteger temp(1);
	*this=*this+temp;
	AdjustBigNum(*this);
	return *this;
}
//����
BigInteger BigInteger::operator --()//����--
{
	int i=1;
	BigInteger temp;
	temp.AddHead(char(i));
	*this=*this-temp;
	AdjustBigNum(*this);
	return *this;
}

//�����������㣬ֻ����������������Ҫ������ʱת��Ϊ��������
BigInteger BigInteger::operator + (BigInteger BigNum2)//����"+"
{ 
	BigInteger result;
	Node *temp1,*temp2;
	int TempNum,rest=0;
	temp1=this->End;//����ʱ�����׵�ַ���õ����������β��
	temp2=BigNum2.End;
	while(temp1 && temp2)
	{
		TempNum=int(temp1->Num)+int(temp2->Num)+rest;//�ڵ���Ԫ����Ӳ����Ͻ�λrest
		if(TempNum>9)//�ж���ӽ���Ƿ�������λ.
		{
			TempNum=TempNum-10;
			rest=1;
		}
		else
			rest=0;
		result.AddHead(char(TempNum));//��������õ����ս��������
		temp1=temp1->Prev;//��������ӵ�����ǰ��һλ
		temp2=temp2->Prev;
	}
	if(temp2)
		temp1=temp2;//������������ʣ�µĲ���
	while(temp1)
	{
		int(TempNum)=int(temp1->Num)+rest;//�ڵ���Ԫ�ؼ��Ͻ�λrest
		if(TempNum>9)
		{
			TempNum=TempNum-10;
			rest=1;
		}
		else
			rest=0;
		result.AddHead(char(TempNum));//��������õ����ս��������
		temp1=temp1->Prev;
	}
	if(rest)
		result.AddHead(char(rest));//�������Ľ�λ�Ƿ����,������������������ײ�.
	AdjustBigNum(result);
	return result;
}

//a-b,Ҫ��a>=b,С-��ʱ���ɴ�-С�ټӸ���,a��b���и���ҪתΪ���Ĵ���
BigInteger BigInteger::operator - (BigInteger BigNum2)//����"-"
{
	BigInteger BigNum1=*this,temp,result;
	Node *temp1,*temp2;
	bool flag;
	int TempNum,rest=0;
	if (int(BigNum1.Head->Num)<0&&int(BigNum2.Head->Num)<0)
	{
		BigNum2.Head->Num=char(int(BigNum2.Head->Num)*(-1));//ת��
		BigNum1.Head->Num=char(int(BigNum1.Head->Num)*(-1));//ת��
		temp=BigNum1;
		BigNum1=BigNum2;
		BigNum2=temp;
	}
	if (int(BigNum1.Head->Num)<0&&int(BigNum2.Head->Num)>0) 
	{
		BigNum1.Head->Num=char(int(BigNum1.Head->Num)*(-1));//ת��
		result=BigNum2+BigNum1;
		result.Head->Num=char(int(result.Head->Num)*(-1));//�为
		return result;
	}
	if (int(BigNum1.Head->Num)>0&&int(BigNum2.Head->Num)<0) 
	{
		BigNum2.Head->Num=char(int(BigNum2.Head->Num)*(-1));//ת��
		result=BigNum2+BigNum1;
		return result;
	}
	
	if (Compare(BigNum1,BigNum2)==1) 
	{
		temp1=BigNum1.End;//����ʱ�����׵�ַ���õ����������β��
		temp2=BigNum2.End;
		flag=true;
	}
	else
	{
		temp1=BigNum2.End;//����ʱ�����׵�ַ���õ����������β��
		temp2=BigNum1.End;
		flag=false;
	}
	
	while(temp1 && temp2)
	{

		TempNum=int(temp1->Num)-int(temp2->Num)+rest;//�ڵ���Ԫ����Ӳ����Ͻ�λrest
		if(TempNum<0)//�ж���ӽ���Ƿ�������λ.
		{
			TempNum=10+TempNum;
			rest=-1;
		}
		else
			rest=0;
		result.AddHead(char(TempNum));//��������õ����ս��������
		temp1=temp1->Prev;//���������������ǰ��һλ
		temp2=temp2->Prev;
	}
	//������������ʣ�µĲ���	
	while(temp1)
	{
		int(TempNum)=int(temp1->Num)+rest;//�ڵ���Ԫ�ؼ��Ͻ�λrest
		if(TempNum<0)
		{
			TempNum=10+TempNum;
			rest=-1;
		}
		else
			rest=0;
		result.AddHead(char(TempNum));//��������õ����ս��������
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


BigInteger BigInteger::operator * (BigInteger BigNum2)//��*��������
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
	while(temp2)//�ɳ����Ĵ�������ж��Ƿ�ȥ�˱�������ÿ��λ
	{
		rest=0;
		//�Գ����е�ÿһλ����һȥ�˱�����
		while(temp1!=NULL)
		{
			TempNum=int(temp1->Num)*int(temp2->Num)+rest;
			if(TempNum>9)
			{ 
				rest=TempNum/10; //��λ����˽����10�������
				TempNum=TempNum%10; //����˽����10��ģȡ��λ
			}
			else
				rest=0;
			temp.AddHead(char(TempNum));//������ʱ����
			temp1=temp1->Prev;
		}
		if(rest!=0)
			temp.AddHead(char(rest));
		for(k=i;k>=1;k--)
			temp.AddEnd(char(0));//�ж�Ӧ����������油����0
		i++; //ÿ�γ�������,������һ�εĲ�0
		temp1=BigNum1.End;//�ѱ����������õ�β,�����ó�����һ��ȥ��ÿ��Ԫ��
		temp2=temp2->Prev;//������ȡ�������ǰ��
		tempa=result.End;//������е��ǽ�ÿ�γ����뱻��������˽���ۼӷŵ�����������ȴ����
		if(result.Head!=NULL)//���������"+"���ػ���һ��,ֻ�Ƕ��˶���ʱ������ÿ�.
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
/* ������ת��Ϊ����������,����˼������:
						div=0;
						while(a>=0)
						{
							a-=b;
							div++;
						}
						div--;	
���ַ�ʽֱ�Ӵ���Ч�ʺܵ�,�����Ľ�Ϊ�õ��̵ĸ�λ����,������������,
�ٶ�����˺ܶ�,���ڿ��Զ����������������Խ���
																		*/
/************************************************************************/
BigInteger BigInteger::operator / (BigInteger BigNum2)//��/�Ž�������
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
		int n,t,i=0,k,sub;//BigNum1,BigNum2�ĳ���
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
		/* �Դ����ε���������ÿ����ȡ�̵ĸ�λ���ֵ���ʽ�õ�,����С��0ʱ�˳�    */
		/************************************************************************/
		while((!(Compare(BigNum1,Bm2)==-1)))
		{
			Bm1=BigNum1;//����ÿ���ı�����
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
			//�Ƚ���������10^(n-t-1)��,����������ô�౶���ɵõ����������λ����0����ֵ
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
			BigNum1=Bm1-BigNum2*Transform(k);//ÿ�����������汻����
			BigNum2=Bm2;//�ָ�����
		}
	}
	AdjustBigNum(result);	
	if (!flag)
	{
		result.Head->Num=char(int(result.Head->Num)*(-1));
	}
	return result;	
}

BigInteger BigInteger::operator % (BigInteger BigNum2)//��%�Ž�������
{
	BigInteger BigNum1,temp,temp1(-1),result;
	BigNum1=*this;
	//Ϊ����Ϊ������,'+'ֻ�ܶ�������������Ч
	while (int(BigNum1.Head->Num)<0)
	{
		BigNum1=BigNum1*temp1;//ת��
		BigNum1=BigNum2-BigNum1;
	}	
	temp=BigNum1/BigNum2;
	result=BigNum1-(temp*BigNum2);
	AdjustBigNum(result);
	return result;	
}
/************************************************************************/
/*������ָ����������,���԰�ģ�ظ�ƽ������˼·����,ֻ�Ǽ�������в���ȡ��*/
/************************************************************************/
BigInteger BigInteger::operator ^ (BigInteger BigNum2)
{
	int count=0;
	BigInteger result(1),temp,temp2(2);
	BigInteger &BigNum1=*this;
	int b[200];
	//��BigNum2ת��Ϊ������
	while(int(BigNum2.Head->Num))
	{
		temp=BigNum2%temp2;
		b[count++]=int(temp.Head->Num);
		BigNum2=BigNum2/temp2;
	}
	//��ģ�ظ�ƽ��������
	for(int i=count-1;i>=0;i--)
	{
		result=result*result;
		if(b[i])
			result=result*BigNum1;
	}
	return result;
}

BigInteger BigInteger::operator = (BigInteger BigNum)//��=�Ž�������
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

CString disp(BigInteger BigNum)//�������
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
//a mod n ����,�����򷵻��棬���򷵻�0
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
//�������ıȽ�:a>b�򷵻�1,��ȷ���0,a<b����-1,ֻ��ԷǸ�����
int Compare(BigInteger BigNum1,BigInteger BigNum2)
{
	int result=0;//��ʼ��Ϊ���
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
//���BigNum==0����1�����򷵻�0
int Compare(BigInteger BigNum)
{
	if (int(BigNum.Head->Num))
	{
		return 0;
	}
	else
		return 1;
}
//�������һ��nλ�Ĵ�����
BigInteger Rand(int n)
{
	BigInteger result,temp1(1);
	int number;
	srand(unsigned(GetTickCount()+seed));	
	number=rand();//�����������
	seed=number;
	srand(number);//�����ӳ�ʼ�����������
	number=1+rand()%9;//����1-9��������Ϊ��һλ
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
	//����ʹ���һλΪ���������ں����������
	if(int(result.End->Num)%2==0)
		result=result+temp1;

	return result;
}
/************************************************************************/
/*     �������������Bignum���ڵĴ�����,��0--BigNum-1֮��
		����: �������һ����ͬλ���Ĵ�����,����ģ                       */
/************************************************************************/
BigInteger Rand(BigInteger BigNum)
{
	Node *p=BigNum.Head;
	BigInteger result;
	int length=0;//BigNum��λ��
	int num;
	while (p)
	{
		length++;
		p=p->Next;
	}
	srand(unsigned(GetTickCount()));	
	randmize=rand()%1500;//�����������
	srand(randmize);
	for(int i=1;i<=length;i++)
	{
		srand(randmize);//�����ӳ�ʼ�����������
		randmize+=rand()%197;//��֤���Ӳ�ͬ
		num=rand()%10;
		result.AddEnd(num);
	}
	AdjustBigNum(result);
	result=result%BigNum;
	return result;
}
//��long�ͱ���ת��Ϊ������BigInteger��
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
		temp=n%10;//�Ӹ�λ��ʼ����ȡ��λ������
		result.AddHead(char(temp));
		n=n/10;
	}
	if (!flag) 
	{
		result.Head->Num=char(int(result.Head->Num)*(-1));//nΪ������ת��Ϊ���Ĵ�����
	}
	return result;
}


//������a^m%n����,��ģ�ظ�ƽ�������
BigInteger Calculate_exp(BigInteger a,BigInteger m,BigInteger n)
{
	int i;
	int count=0;
	BigInteger d(1),temp,temp2(2);
	int b[400];
	//����mת��Ϊ������
	while(int(m.Head->Num))
	{
		temp=m%temp2;
		b[count++]=int(temp.Head->Num);
		m=m/temp2;
	}
	//��ģ�ظ�ƽ��������
	for(i=count-1;i>=0;i--)
	{
		d=d*d%n;
		if(b[i])
			d=d*(a%n)%n;
	}
	return d;
}
/************************************************************************/
/*���Լ���֮ǰ��ɸѡ,������100���ڵ�С����								*/
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
/* Rabin_Miller���Լ���													*/
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
		b=temp2+Rand(n-temp2-temp2);//���ѡȡ2<=b<=n-2
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
//����mλ��α���������
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
//��һ������ת��Ϊ������
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

