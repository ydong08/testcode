//Download by http://www.NewXing.com
#if !defined(AFX_MINGWENDLG_H__4C809E38_A4B2_4778_90D3_48D336B5A78C__INCLUDED_)
#define AFX_MINGWENDLG_H__4C809E38_A4B2_4778_90D3_48D336B5A78C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MINGWENDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMINGWENDlg dialog

class CMINGWENDlg : public CDialog
{
// Construction
public:
	CMINGWENDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMINGWENDlg)
	enum { IDD = IDD_DIALOG_MINGWEN };
	CString	m_MingWen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMINGWENDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMINGWENDlg)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINGWENDLG_H__4C809E38_A4B2_4778_90D3_48D336B5A78C__INCLUDED_)
