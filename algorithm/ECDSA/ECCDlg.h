//Download by http://www.NewXing.com
#if !defined(AFX_ECCDLG_H__00C9586F_80ED_4475_950C_EC5D62E279B6__INCLUDED_)
#define AFX_ECCDLG_H__00C9586F_80ED_4475_950C_EC5D62E279B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ECCDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CECCDlg dialog

class CECCDlg : public CDialog
{
// Construction
public:
	CECCDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CWnd *pCWnd;
// Dialog Data
	//{{AFX_DATA(CECCDlg)
	enum { IDD = IDD_DIALOG_ECC };
	CString	m_PrimeNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CECCDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECCDLG_H__00C9586F_80ED_4475_950C_EC5D62E279B6__INCLUDED_)
