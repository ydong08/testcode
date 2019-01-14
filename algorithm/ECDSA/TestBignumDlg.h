//Download by http://www.NewXing.com
#if !defined(AFX_TESTBIGNUMDLG_H__C76CE341_FC15_4202_B83D_9656797213EB__INCLUDED_)
#define AFX_TESTBIGNUMDLG_H__C76CE341_FC15_4202_B83D_9656797213EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestBignumDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestBignumDlg dialog

class CTestBignumDlg : public CDialog
{
// Construction
public:
	CTestBignumDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestBignumDlg)
	enum { IDD = IDD_DIALOG_BIGNUM };
	int		m_BigNum1;
	int		m_BigNum2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestBignumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestBignumDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTBIGNUMDLG_H__C76CE341_FC15_4202_B83D_9656797213EB__INCLUDED_)
