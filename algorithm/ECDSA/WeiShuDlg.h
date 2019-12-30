//Download by http://www.NewXing.com
#if !defined(AFX_WEISHUDLG_H__5A553197_8212_43F7_99B3_DC4459618279__INCLUDED_)
#define AFX_WEISHUDLG_H__5A553197_8212_43F7_99B3_DC4459618279__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WeiShuDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWeiShuDlg dialog

class CWeiShuDlg : public CDialog
{
// Construction
public:
	CWeiShuDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWeiShuDlg)
	enum { IDD = IDD_DIALOG_WEISHU };
	int		m_WeiShu;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeiShuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWeiShuDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEISHUDLG_H__5A553197_8212_43F7_99B3_DC4459618279__INCLUDED_)
