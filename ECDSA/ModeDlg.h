//Download by http://www.NewXing.com
#if !defined(AFX_MODEDLG_H__DF6CCBC2_4F54_4505_9457_0658E155AD4F__INCLUDED_)
#define AFX_MODEDLG_H__DF6CCBC2_4F54_4505_9457_0658E155AD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModeDlg dialog

class CModeDlg : public CDialog
{
// Construction
public:
	CModeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModeDlg)
	enum { IDD = IDD_DIALOG_MODE };
	CString	m_Base;
	CString	m_Mode;
	CString	m_Exp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModeDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODEDLG_H__DF6CCBC2_4F54_4505_9457_0658E155AD4F__INCLUDED_)
