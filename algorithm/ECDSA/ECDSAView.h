//Download by http://www.NewXing.com
// ECDSAView.h : interface of the CECDSAView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECDSAVIEW_H__E1AED494_E4E9_41AD_82BB_F913B7D65F89__INCLUDED_)
#define AFX_ECDSAVIEW_H__E1AED494_E4E9_41AD_82BB_F913B7D65F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/************************************************************************/
/* 全局参数函数的相关说明												*/
/************************************************************************/
#define  MAX 0xffffff

struct Point 
{
	BigInteger x;
	BigInteger y;
};

void Elliptic_Curve(BigInteger &a,BigInteger &b,BigInteger &q,int m);//生成椭圆曲线
Point Point_Calculate(Point P,Point Q);//点加和倍点加
Point Point_Multiply(Point P,BigInteger n);//点的数乘nP
void BasePoint(Point &G,BigInteger &n);//查找基点并计算其阶

void SetKey(BigInteger &d,Point &Q);//生成密钥对
bool CheckKey();//检验密钥

void Sign();//签名过程
bool Authentic();//认证过程

class CECDSAView : public CEditView
{
protected: // create from serialization only
	CECDSAView();
	DECLARE_DYNCREATE(CECDSAView)

// Attributes
public:
	CECDSADoc* GetDocument();

// Operations
public:

	CString str;
	bool flag_ecc;
	bool flag_base;
	bool flag_key,flag_sign;
	CWnd *pCWnd;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECDSAView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CECDSAView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CECDSAView)
	afx_msg void OnMenuEcc();
	afx_msg void OnMenuBasepoint();
	afx_msg void OnMenuKey();
	afx_msg void OnMenuSignature();
	afx_msg void OnMenuAuthentic();
	afx_msg void OnEcc();
	afx_msg void OnBase();
	afx_msg void OnKey();
	afx_msg void OnSign();
	afx_msg void OnAUTHENTIC();
	afx_msg void OnMenuTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ECDSAView.cpp
inline CECDSADoc* CECDSAView::GetDocument()
   { return (CECDSADoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECDSAVIEW_H__E1AED494_E4E9_41AD_82BB_F913B7D65F89__INCLUDED_)
