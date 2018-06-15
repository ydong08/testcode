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


/*用双链表实现大整数的处理*/
struct Node        //定义了节点的结构
{
	char Num;
	Node *Prev,*Next;//前驱和后继指针
};

static long randmize;//用作随机数的种子

class BigInteger      //定义BigInteger 类
{
private:
	Node *Head,*End,*TempNode;
	void AddHead(char Num);//头插入
	void AddEnd(char Num);//尾插入
public:
	BigInteger();
	BigInteger(char i);
	BigInteger(const BigInteger &BigNum);
	~BigInteger();	

	friend void AdjustBigNum(BigInteger &BigNum);//调整大整数,消去前面不必要的0
	
	void BigNumToIndex(int b[],int &count);//大整数转化为二进制数
	
	/*如下的运算符重载不会改变参加运算的数*/
	BigInteger operator ++();//重载自增运算
	BigInteger operator --();//重载自减运算

	//大整数加运算，只能是两正整数，需要时转化为-处理
	BigInteger operator + (BigInteger BigNum);

	//大整数减运算，两整数的正负可任意
	BigInteger operator - (BigInteger BigNum);

	//大整数乘运算，可正负
	BigInteger operator * (BigInteger BigNum);

	//大整数除运算，可正负
	BigInteger operator / (BigInteger BigNum);

	//大整数模运算，可正负
	BigInteger operator % (BigInteger BigNum);
	
	//大整数幂运算，当然只能都是正整数
	BigInteger operator ^ (BigInteger BigNum);

	//大整数赋值运算，可正负
	BigInteger operator = (BigInteger BigNum);
	
	//大整数的比较:a>b则返回1,相等返回0,a<b返回-1,只针对非负整数
	friend int Compare(BigInteger BigNum1,BigInteger BigNum2);

	//如果BigNum==0返回1，否则返回0
	friend int Compare(BigInteger BigNum);

	//将大整数转化为字符串,以便在文档中输出
	friend CString disp(BigInteger BigNum);

	//求a mod n的逆,存在则返回逆元,否则返回0
	friend BigInteger Inv(BigInteger a,BigInteger n);

	//随机产生n位的大整数
	friend BigInteger Rand(int n);

	//随机产生大整数Bignum以内的大整数,即0--BigNum-1之间
	friend BigInteger Rand(BigInteger BigNum);
	
	//将int型变量转化为大整数BigInteger型
	friend BigInteger Transform(long n);

	//大整数a^m%n运算,直接用a^m,用模重复平方法求解
	friend BigInteger Calculate_exp(BigInteger a,BigInteger m,BigInteger n);
	
	//用素性检验之前先进行筛选
	friend bool Select(BigInteger BigNum, int n);

	//M_Rabin素性检验
	friend bool M_Rabin(BigInteger n,int k);

	//生成一个m位的伪随机素数
	friend BigInteger Produce_Prime(int m);	

	//将字符串转化为大整数,以便后面从对话框中得到大整数
	friend BigInteger CStringToBignum(CString);
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECDSA_H__2D37D388_5CC5_4664_9D85_B33F989F8233__INCLUDED_)
