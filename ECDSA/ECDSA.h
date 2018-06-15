//Download by http://www.NewXing.com
// ECDSA.h : main header file for the ECDSA application
//

#if !defined(AFX_ECDSA_H__2D37D388_5CC5_4664_9D85_B33F989F8233__INCLUDED_)
#define AFX_ECDSA_H__2D37D388_5CC5_4664_9D85_B33F989F8233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include "stdafx.h" before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CECDSAApp:
// See ECDSA.cpp for the implementation of this class
//

class CECDSAApp : public CWinApp
{
public:
	CECDSAApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECDSAApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CECDSAApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/*��˫����ʵ�ִ������Ĵ���*/
struct Node        //�����˽ڵ�Ľṹ
{
	char Num;
	Node *Prev,*Next;//ǰ���ͺ��ָ��
};

static long randmize;//���������������

class BigInteger      //����BigInteger ��
{
private:
	Node *Head,*End,*TempNode;
	void AddHead(char Num);//ͷ����
	void AddEnd(char Num);//β����
public:
	BigInteger();
	BigInteger(char i);
	BigInteger(const BigInteger &BigNum);
	~BigInteger();	

	friend void AdjustBigNum(BigInteger &BigNum);//����������,��ȥǰ�治��Ҫ��0
	
	void BigNumToIndex(int b[],int &count);//������ת��Ϊ��������
	
	/*���µ���������ز���ı�μ��������*/
	BigInteger operator ++();//������������
	BigInteger operator --();//�����Լ�����

	//�����������㣬ֻ����������������Ҫʱת��Ϊ-����
	BigInteger operator + (BigInteger BigNum);

	//�����������㣬������������������
	BigInteger operator - (BigInteger BigNum);

	//�����������㣬������
	BigInteger operator * (BigInteger BigNum);

	//�����������㣬������
	BigInteger operator / (BigInteger BigNum);

	//������ģ���㣬������
	BigInteger operator % (BigInteger BigNum);
	
	//�����������㣬��Ȼֻ�ܶ���������
	BigInteger operator ^ (BigInteger BigNum);

	//��������ֵ���㣬������
	BigInteger operator = (BigInteger BigNum);
	
	//�������ıȽ�:a>b�򷵻�1,��ȷ���0,a<b����-1,ֻ��ԷǸ�����
	friend int Compare(BigInteger BigNum1,BigInteger BigNum2);

	//���BigNum==0����1�����򷵻�0
	friend int Compare(BigInteger BigNum);

	//��������ת��Ϊ�ַ���,�Ա����ĵ������
	friend CString disp(BigInteger BigNum);

	//��a mod n����,�����򷵻���Ԫ,���򷵻�0
	friend BigInteger Inv(BigInteger a,BigInteger n);

	//�������nλ�Ĵ�����
	friend BigInteger Rand(int n);

	//�������������Bignum���ڵĴ�����,��0--BigNum-1֮��
	friend BigInteger Rand(BigInteger BigNum);
	
	//��int�ͱ���ת��Ϊ������BigInteger��
	friend BigInteger Transform(long n);

	//������a^m%n����,ֱ����a^m,��ģ�ظ�ƽ�������
	friend BigInteger Calculate_exp(BigInteger a,BigInteger m,BigInteger n);
	
	//�����Լ���֮ǰ�Ƚ���ɸѡ
	friend bool Select(BigInteger BigNum, int n);

	//M_Rabin���Լ���
	friend bool M_Rabin(BigInteger n,int k);

	//����һ��mλ��α�������
	friend BigInteger Produce_Prime(int m);	

	//���ַ���ת��Ϊ������,�Ա����ӶԻ����еõ�������
	friend BigInteger CStringToBignum(CString);
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECDSA_H__2D37D388_5CC5_4664_9D85_B33F989F8233__INCLUDED_)
