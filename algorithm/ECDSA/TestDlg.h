//Download by http://www.NewXing.com
#if !defined(AFX_TESTDLG_H__FC581BFD_7AE4_41FE_B9FC_01D042B61DC1__INCLUDED_)
#define AFX_TESTDLG_H__FC581BFD_7AE4_41FE_B9FC_01D042B61DC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);   // standard constructor

	CWnd *pCWnd;
	int flag;
// Dialog Data
	//{{AFX_DATA(CTestDlg)
	enum { IDD = IDD_DIALOG_TEST };
	CButton	m_RadioMultiply;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLG_H__FC581BFD_7AE4_41FE_B9FC_01D042B61DC1__INCLUDED_)
