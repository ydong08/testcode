//Download by http://www.NewXing.com
// ECDSAView.cpp : implementation of the CECDSAView class
//

#include "stdafx.h"
#include "ECDSA.h"


#include "ECDSADoc.h"
#include "ECDSAView.h"
#include "ECCDlg.h"
#include "MINGWENDlg.h"
#include "TestDlg.h"
#include "TestBignumDlg.h"
#include "ModeDlg.h"
#include "WeiShuDlg.h"
#include "time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECDSAView

IMPLEMENT_DYNCREATE(CECDSAView, CEditView)

BEGIN_MESSAGE_MAP(CECDSAView, CEditView)
	//{{AFX_MSG_MAP(CECDSAView)
	ON_COMMAND(ID_MENU_ECC, OnMenuEcc)
	ON_COMMAND(ID_MENU_BASEPOINT, OnMenuBasepoint)
	ON_COMMAND(ID_MENU_KEY, OnMenuKey)
	ON_COMMAND(ID_MENU_SIGNATURE, OnMenuSignature)
	ON_COMMAND(ID_MENU_AUTHENTIC, OnMenuAuthentic)
	ON_COMMAND(ID_ECC, OnEcc)
	ON_COMMAND(ID_BASE, OnBase)
	ON_COMMAND(ID_KEY, OnKey)
	ON_COMMAND(ID_SIGN, OnSign)
	ON_COMMAND(AUTHENTIC, OnAUTHENTIC)
	ON_COMMAND(ID_MENU_TEST, OnMenuTest)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECDSAView construction/destruction


//ȫ�ֱ����Ķ���
BigInteger a,b,q,n;
Point G;
BigInteger d;
Point Q;
BigInteger r,s;
BigInteger m;


CECDSAView::CECDSAView()
{
	// TODO: add construction code here
	flag_base=false;
	flag_ecc=false;
	flag_key=false;
	flag_sign=false;
}

CECDSAView::~CECDSAView()
{
}

BOOL CECDSAView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CECDSAView drawing

void CECDSAView::OnDraw(CDC* pDC)
{
	CECDSADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CECDSAView printing

BOOL CECDSAView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CECDSAView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CECDSAView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CECDSAView diagnostics

#ifdef _DEBUG
void CECDSAView::AssertValid() const
{
	CEditView::AssertValid();
}

void CECDSAView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CECDSADoc* CECDSAView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CECDSADoc)));
	return (CECDSADoc*)m_pDocument;
}
#endif //_DEBUG

/************************************************************************/
/*       ����ȫ�ֲ�����һЩ������ʵ��                                   */
/************************************************************************/

void Elliptic_Curve(BigInteger &a,BigInteger &b,BigInteger &q,int m)
{
	BigInteger temp2(2);
	q=Rand(m);
	
	while (!M_Rabin(q,9)) 
	{
		q=q-temp2;
	}
	a=Rand(q);
	b=Rand(q/(temp2*temp2*temp2));
}
//������㣬���������
void BasePoint(Point &G,BigInteger &n)
{
	BigInteger aa,temp0(0),temp1(1),temp2(2),temp3(3),temp4(4);
	BigInteger temp=Transform(MAX),temp11=temp4*temp2+temp3;

	G.x=Rand(q);
	Point R;
	while (Compare(G.x,q-temp1)==-1)
	{
		aa=(Calculate_exp(G.x,3,q)+a*G.x%q+b)%q;
		if (Compare(aa))
		{
			G.y=temp0;
		}
		else
		{                                                                                     
			if (Compare(Calculate_exp(aa,(q-temp1)/temp2,q),temp1)==0)
			{
				if (Compare(q%temp4,temp3)==0)
				{
					G.y=Calculate_exp(aa,(q+temp1)/temp4,q);
				}
				else
				{
					if (Compare(Calculate_exp(aa,(q-temp1)/temp4,q),temp1)==0) 
					{
						G.y=Calculate_exp(aa,(q+temp3)/(temp4+temp4),q);
					}
					else if (Compare(Calculate_exp(aa,(q-temp1)/temp4,q),q-temp1)==0)
					{
						G.y=Calculate_exp(temp2,(q-temp1)/temp4,q)*Calculate_exp(aa,(q+temp3)/(temp4+temp4),q)%q;
					}
					else
					{
						G.x=G.x+temp1;
						continue;
					}
				}
			}
			else
			{
				G.x=G.x+temp1;
				continue;
			}
		}
		n=temp1;
		R.x=G.x;R.y=G.y;
		
		while (Compare(G.x,q-temp1)==-1)
		{
			R=Point_Calculate(R,G);
			++n;
			if (Compare(n,temp)==1) 
			{
				G.x=G.x+temp3*temp3;break;
			}
			if (Compare(R.x)&&Compare(R.y))
			{
				if (M_Rabin(n,10))
				{
					return;
				}
				else
				{
					G.x=G.x+temp3*temp3;
					break;
				}
			}		
		}
	
	}
	n=temp0;
}
//��Ӻͱ����
Point Point_Calculate(Point P,Point Q)
{
	BigInteger r,temp0(0),temp2(2),temp3(3);
	Point R;
	if (Compare(P.x)&&Compare(P.y))
	{
		R=Q;return R;
	}
	else if (Compare(Q.x)&&Compare(Q.y))
	{
		R=P;return R;
	}
	else if (Compare(P.x,Q.x)) 
	{
		r=(Q.y-P.y)*Inv(Q.x-P.x,q)%q;
	}
	else if (Compare(P.y,Q.y)) 
	{
		R.x=R.y=temp0;return R;
	}
	else if (Compare(Q.y))
	{
		R.x=R.y=temp0;return R;
	}
	else
	{
		r=(temp3*P.x*P.x+a)*Inv(temp2*P.y,q)%q;
	}

	R.x=(r*r%q-P.x-Q.x)%q;
	if (Compare(R.x,temp0)==-1)
	{
		R.x=R.x+q;
	}
	R.y=(r*(P.x-R.x)-P.y)%q;
	if (Compare(R.y,temp0)==-1)
	{
		R.y=R.y+q;
	}
	return R;
}
//��������nP,�����ظ�ƽ��������
Point Point_Multiply(Point P,BigInteger n)
{
	Point result;
	BigInteger temp,temp2(2),temp0(0);
	int b[160],count;
	result.x=result.y=temp0;//��ʼ��ΪO��
	n.BigNumToIndex(b,count);//�õ�n�Ķ����Ʊ�ʾ
	//���ظ�ƽ�������nP
	for(int i=count-1;i>=0;i--)
	{
		result=Point_Calculate(result,result);
		if (b[i])
		{
			result=Point_Calculate(result,P);
		}
	}
	return result;
}
//������Կ��
void SetKey(BigInteger &d,Point &Q)
{
	
	bool r=false;
	BigInteger temp2(2),temp1(1);
	srand(unsigned(GetTickCount()));
	d=Transform(rand()%47);
	BigInteger nn=n-temp1;
	while (Compare(d,nn)==-1)
	{
		Q=Point_Multiply(G,d);//Q=dG
		r=CheckKey();
		if (r)
		{
			return;
		}
		else
			++d;
	}
	BigInteger temp0(0);
	d=temp0;
}
//������Կ
bool CheckKey()
{
	BigInteger temp0(0),temp3(3),temp2(2),temp1(1);
	if (Compare(Q.x)&&Compare(Q.y))//Q=O
	{
		return false;
	}
	int f1=Compare(Q.x,temp0);
	int f2=Compare(Q.x,q-temp1);
	if ((f1==-1)||(f2==1)) //f1<0||f2>=q
	{
		return false;
	}
	f1=Compare(Q.y,temp0);
	f2=Compare(Q.y,q-temp1);
	if ((f1==-1)||(f2==1)) //f1<0||f2>=q
	{
		return false;
	}
	BigInteger aa=(Calculate_exp(Q.x,temp3,q)+a*Q.x%q+b)%q;
	BigInteger bb=Calculate_exp(Q.y,temp2,q);
	if (Compare(aa,bb))
	{
		return false;
	}
	Point R=Point_Multiply(Q,n);//R=nQ=ndG
	if (Compare(R.x)&&Compare(R.y))//R=O
	{
		return true;
	}
	else
	{
		return false;
	}
}
//ǩ��
void Sign()
{
	Point P;
	BigInteger k,kv,temp1(1);
	srand(unsigned(GetTickCount()));
	k=temp1+Rand(n-temp1);
	while (1)
	{
		P=G;
		kv=Inv(k,n);
		if (Compare(kv))
		{
			++k;continue;
		}
		P=Point_Multiply(G,k);//P=kG
		r=P.x%n;
		if (!Compare(r))
		{
			s=kv*(m+d*r%n)%n;
			if (!Compare(s))
			{
				break;
			}
			else
			{
				++k;
				continue;
			}
		}
		else
		{
			++k;
		}
	}
}
//��֤
bool Authentic()
{
	BigInteger temp1(1);
	int f1=Compare(r,temp1);
	int f2=Compare(r,n-temp1);
	if ((f1==-1)||(f2==1)) 
	{
		return false;
	}
	f1=Compare(s,temp1);
	f2=Compare(s,n-temp1);
	if ((f1==-1)||(f2==1)) 
	{
		return false;
	}
	BigInteger w=Inv(s,n);
	CString str1=disp(w);
	if (Compare(w)) 
	{
		return false;
	}
	BigInteger u1=(m*w)%n;
	BigInteger u2=(r*w)%n;
	Point X1,X2,X;
	X1=Point_Multiply(G,u1);//X1=u1*G
	X2=Point_Multiply(Q,u2);//X2=u2*Q
	X=Point_Calculate(X1,X2);
	if (Compare(X.x)&&Compare(X.y))
	{
		return false;
	}
	BigInteger v=X.x%n;
	if (Compare(v,r))
	{
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CECDSAView message handlers

void CECDSAView::OnMenuEcc() 
{
	// TODO: Add your command handler code here
	CECCDlg ECCDlg(this);
	CString str1;
	ECCDlg.DoModal();
	pCWnd=ECCDlg.pCWnd;
	int m=atoi(ECCDlg.m_PrimeNum);
	if (m)
	{
		str +="----����������Բ���߲���----\r\n";
		pCWnd->SetWindowText(str);
		pCWnd->UpdateWindow();//ˢ�´���

		Elliptic_Curve(a,b,q,m);

		str+="    a=";
		str1=disp(a);
		str+=str1;
		pCWnd->SetWindowText(str);
		str+="    b=";
		str1=disp(b);
		str+=str1;
		pCWnd->SetWindowText(str);
		str+="    q=";
		str1=disp(q);
		str+=str1;
		pCWnd->SetWindowText(str);
		str +="----��Բ���߲����������----\r\n\r\n";
		pCWnd->SetWindowText(str);
		flag_ecc=true;
	}
	else
	{
		return;
		//MessageBox("��û������������λ��");
	}
}

void CECDSAView::OnMenuBasepoint() 
{
	// TODO: Add your command handler code here
	CString str1;
	if (!flag_ecc)
	{
		MessageBox("��û��������Բ����");
		return;
	}
	else
	{
		str +="****�������ɻ�������****\r\n";
		pCWnd->SetWindowText(str);
		pCWnd->UpdateWindow();//ˢ�´���
		
		BasePoint(G,n);
		
		if (Compare(n))
		{
			MessageBox("�����߲���������");
			str="";
			UpdateData(TRUE);
			pCWnd->SetWindowText(str);
			UpdateData(FALSE);
			return;
		}
		str+="    G.x=";
		str1=disp(G.x);
		str+=str1;
		pCWnd->SetWindowText(str);
		str+="    G.y=";
		str1=disp(G.y);
		str+=str1;
		pCWnd->SetWindowText(str);
		str+="    n=";
		str1=disp(n);
		str+=str1;
		pCWnd->SetWindowText(str);
		str +="****���������������****\r\n\r\n";
		pCWnd->SetWindowText(str);
		flag_base=true;
	}
}

void CECDSAView::OnMenuKey() 
{
	// TODO: Add your command handler code here
	CString str1;
	if (!flag_base)
	{
		MessageBox("��û�����ɻ���");
		return;
	}
	else
	{
		str+="----����������Կ��---\r\n";
		pCWnd->SetWindowText(str);
		pCWnd->UpdateWindow();//ˢ�´���

		SetKey(d,Q);

		if (Compare(d)) 
		{
			MessageBox("�����߲���������");
			str="";
			UpdateData(TRUE);
			pCWnd->SetWindowText(str);
			UpdateData(FALSE);
			return;
		}
		str+="    d=";
		str1=disp(d);
		str+=str1;
		pCWnd->SetWindowText(str);
		str+="    Q.x=";
		str1=disp(Q.x);
		str+=str1;
		pCWnd->SetWindowText(str);
		str+="    Q.y=";
		str1=disp(Q.y);
		str+=str1;
		pCWnd->SetWindowText(str);
		str+="----��Կ���������---\r\n\r\n";
		pCWnd->SetWindowText(str);
		flag_key=true;
	}
}

void CECDSAView::OnMenuSignature() 
{
	// TODO: Add your command handler code here
	CMINGWENDlg MINGWENDlg(this);
	MINGWENDlg.DoModal();
	CString str0=MINGWENDlg.m_MingWen;
	TCHAR c;
	//�����ǶԻ�ȡ�����Ľ���ɢ�д���
	if (str0.IsEmpty())
	{
		//MessageBox("��û����������������");
		return;
	}
	long mm=0xff;
	int length=str0.GetLength();
	int i;
	for(i=0;i<length;i++)
	{
		c=str0[i];
		mm|=c;
		i++;
	}
	m=Transform(mm);

	CString str1;
	if (!flag_key)
	{
		MessageBox("��û��������Կ��");
		return;
	}
	else
	{
		str+="****����ǩ��****\r\n";
		pCWnd->SetWindowText(str);
		pCWnd->UpdateWindow();//ˢ�´���
		
		Sign();
	
		str+="    r=";
		str1=disp(r);
		str+=str1;
		pCWnd->SetWindowText(str);
		str+="    s=";
		str1=disp(s);
		str+=str1;
		pCWnd->SetWindowText(str);
		str+="****ǩ�����****\r\n\r\n";
		pCWnd->SetWindowText(str);
		flag_sign=true;
	}
}

void CECDSAView::OnMenuAuthentic() 
{
	// TODO: Add your command handler code here
	if (!flag_sign)
	{
		MessageBox("��û��������Կ��");
	}
	else
	{
		str+="----���ڶ�ǩ����֤----\r\n";
		pCWnd->SetWindowText(str);
		pCWnd->UpdateWindow();//ˢ�´���
		
		bool flag=Authentic();
		
		if (flag)
		{
			str+="----��֤�ɹ�----\r\n";
			pCWnd->SetWindowText(str);
		}
		else
		{
			str+="----��֤ʧ��----\r\n";
			pCWnd->SetWindowText(str);
		}
	}	
}

void CECDSAView::OnEcc() 
{
	// TODO: Add your command handler code here
	OnMenuEcc();
}

void CECDSAView::OnBase() 
{
	// TODO: Add your command handler code here
	OnMenuBasepoint();
}

void CECDSAView::OnKey() 
{
	// TODO: Add your command handler code here
	OnMenuKey();
}

void CECDSAView::OnSign() 
{
	// TODO: Add your command handler code here
	OnMenuSignature();
}

void CECDSAView::OnAUTHENTIC() 
{
	// TODO: Add your command handler code here
	OnMenuAuthentic();
}


void CECDSAView::OnMenuTest() 
{
	// TODO: Add your command handler code here
	CTestDlg TestDlg(this);
	TestDlg.DoModal();
	int flag=TestDlg.flag;
	pCWnd=TestDlg.pCWnd;
	int number;
	CString str1;
	switch(flag)
	{
	/************************************************************************/
	/* �������˷�����                                                       */
	/************************************************************************/
	case 1:
		{
			CTestBignumDlg TestBignumDlg(this);
			char s[30];
			BigInteger BigNum1,BigNum2;
			TestBignumDlg.DoModal();
			number=TestBignumDlg.m_BigNum1;
			str="";
			str+="----�������˷�����ʵ��----\r\n\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���
			
			memset(s,'\0',30);
			sprintf(s,"���������%dλ����A:\r\n",number);
			str+=s;
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();
			BigNum1=Rand(number);
			str1=disp(BigNum1);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���

			memset(s,'\0',30);
			number=TestBignumDlg.m_BigNum2;
			sprintf(s,"���������%dλ����B:\r\n",number);
			str+=s;
			pCWnd->SetWindowText(str);
			BigNum2=Rand(number);
			str1=disp(BigNum2);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���

			str+="��˵Ľ����:\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���
			BigNum1=BigNum1*BigNum2;
			str1=disp(BigNum1);
			str+=str1;
			pCWnd->SetWindowText(str);
			
			break;
		}
	/************************************************************************/
	/* ��������������                                                       */
	/************************************************************************/
	case 2:
		{
			CTestBignumDlg TestBignumDlg(this);
			char s[30];
			BigInteger BigNum1,BigNum2;
			TestBignumDlg.DoModal();
			number=TestBignumDlg.m_BigNum1;
			str="";
			str+="----��������������ʵ��----\r\n\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���
			
			memset(s,'\0',30);
			sprintf(s,"���������%dλ����A:\r\n",number);
			str+=s;
			pCWnd->SetWindowText(str);						
			BigNum1=Rand(number);
			str1=disp(BigNum1);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���
			
			memset(s,'\0',30);
			number=TestBignumDlg.m_BigNum2;
			sprintf(s,"���������%dλ����B:\r\n",number);
			str+=s;
			pCWnd->SetWindowText(str);
			BigNum2=Rand(number);
			str1=disp(BigNum2);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���
			
			BigNum1=BigNum1/BigNum2;
			str+="����Ľ����:\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���
			str1=disp(BigNum1);
			str+=str1;
			pCWnd->SetWindowText(str);
			break;
		}
	/************************************************************************/
	/* ģ���㷨����                                                         */
	/************************************************************************/
	case 3:
		{
			CModeDlg ModeDlg(this);
			ModeDlg.DoModal();
			BigInteger BigNum1,BigNum2;
			CString sub;
			sub=ModeDlg.m_Base;
			if (sub.IsEmpty())
			{
				MessageBox("��û����������������");
				return;
			}
			str="";
			str+="-----ģ���㷨����ʵ��a mod n����-----\r\n\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���

			
			BigNum1=CStringToBignum(sub);
			str+="a=";
			str1=disp(BigNum1);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���

			sub=ModeDlg.m_Mode;
			BigNum2=CStringToBignum(sub);
			str+="n=";
			str1=disp(BigNum2);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���

			BigNum1=Inv(BigNum1,BigNum2);
			if (Compare(BigNum1))
			{
				MessageBox("������������,û����Ԫ");
				str="";
				pCWnd->SetWindowText(str);
				return;
			}
		
			str+="a mod n����=";
			str1=disp(BigNum1);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);

			break;
		}
	/************************************************************************/
	/* ָ��ģ�������                                                       */
	/************************************************************************/
	case 4:
		{
			CModeDlg ModeDlg(this);
			ModeDlg.DoModal();
			BigInteger BigNum1,BigNum2,BigNum3;
			CString sub;
			sub=ModeDlg.m_Base;
			if (sub.IsEmpty())
			{
				MessageBox("��û����������������");
				return;
			}
			str="";
			str+="-----ָ��ģ�㷨����ʵ��a^m mod n-----\r\n\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���
			
			
			BigNum1=CStringToBignum(sub);
			str+="a=";
			str1=disp(BigNum1);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���
			
			sub=ModeDlg.m_Exp;
			BigNum2=CStringToBignum(sub);
			str+="m=";
			str1=disp(BigNum2);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���

			sub=ModeDlg.m_Mode;
			BigNum3=CStringToBignum(sub);
			str+="n=";
			str1=disp(BigNum3);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���
			
			BigNum1=Calculate_exp(BigNum1,BigNum2,BigNum3);
			str+="a^m mod n=";
			str1=disp(BigNum1);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			break;
		
		}
		/************************************************************************/
		/* ������ɴ�����                                                       */
		/************************************************************************/
	case 5:
		{
			CWeiShuDlg WeishuDlg(this);
			BigInteger BigNum1;
			char s[30];
			WeishuDlg.DoModal();
			int Weishu=WeishuDlg.m_WeiShu;
			if (!Weishu) 
			{
				MessageBox("û������Ҫ���ɵĴ�������λ��");
				return;
			}
			else
			{
				str="";
				str+="-----������ɴ�����������ʵ��-----\r\n\r\n";
				pCWnd->SetWindowText(str);
				memset(s,'\0',30);
				sprintf(s,"���������%dλ����:\r\n",Weishu);
				str+=s;
				pCWnd->UpdateWindow();//ˢ�´���
				
				BigNum1=Rand(Weishu);
				str1=disp(BigNum1);
				str+=str1;
				str+="\r\n";
				pCWnd->SetWindowText(str);
			}
			break;
		}
	/************************************************************************/
	/* ����α����������Ĳ���                                               */
	/************************************************************************/
	case 6:
		{
			CWeiShuDlg WeishuDlg(this);
			BigInteger BigNum1;
			char s[30];
			WeishuDlg.DoModal();
			int Weishu=WeishuDlg.m_WeiShu;
			if (!Weishu) 
			{
				MessageBox("û������Ҫ���ɵĴ�������λ��");
				return;
			}
			else
			{
				str="";
				str+="-----����α���������������ʵ��-----\r\n\r\n";
				pCWnd->SetWindowText(str);
				pCWnd->UpdateWindow();//ˢ�´���
				BigNum1=Produce_Prime(Weishu);
				memset(s,'\0',30);
				sprintf(s,"���������%dλ������:\r\n",Weishu);
				str+=s;
				pCWnd->UpdateWindow();//ˢ�´���
				str1=disp(BigNum1);
				str+=str1;
				str+="\r\n";
				pCWnd->SetWindowText(str);				
			}
			break;
		}
		/************************************************************************/
		/*��������㷨�Ľ�ǰ��Աȵ�����ʵ��									*/
		/************************************************************************/
	case 7:
		{
			q=Transform(5987);
			a=Transform(1164);
			b=Transform(17);
			G.x=Transform(68);
			G.y=Transform(733);
			n=Transform(37);
			char sub[30];
			str="";
			str+="-----�����㷨�Ľ�ǰ��Աȵ�����ʵ��-----\r\n\r\n";
			str+="G.x=";
			str1=disp(G.x);
			str+=str1;
			str+="G.x=";
			str1=disp(G.y);
			str+=str1;
			str+="\r\n";
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���

			time_t tBegin1,tEnd1,tBegin2,tEnd2;
			double time_cost;
			long nn=150;
			BigInteger mm=Transform(nn),temp1(1);
			Point P;
			P.x=G.x;
			P.y=G.y;
			time(&tBegin1);
			for(int i=150;i>1;i--)
			{
				P=Point_Calculate(P,G);
			}
			time(&tEnd1);
			time_cost=difftime(tEnd1,tBegin1);
			memset(sub,'\0',30);
			sprintf(sub,"�����ۼӷ�����150G��ʱ%.01f��\r\n",time_cost);
			str+=sub;
			pCWnd->SetWindowText(str);
			pCWnd->UpdateWindow();//ˢ�´���
			
			time(&tBegin2);
			P=Point_Multiply(G,mm);
			time(&tEnd2);
			time_cost=difftime(tEnd2,tBegin2);
			memset(sub,'\0',30);
			sprintf(sub,"����ģ�ظ�ƽ��������150G��ʱ%.01f��\r\n",time_cost);
			str+=sub;
			pCWnd->SetWindowText(str);
			break;
		}	
	}
}
