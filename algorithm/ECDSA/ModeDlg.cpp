//Download by http://www.NewXing.com
// ModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECDSA.h"
#include "ModeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModeDlg dialog


CModeDlg::CModeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModeDlg)
	m_Base = _T("");
	m_Mode = _T("");
	m_Exp = _T("");
	//}}AFX_DATA_INIT
}


void CModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModeDlg)
	DDX_Text(pDX, IDC_EDIT1, m_Base);
	DDX_Text(pDX, IDC_EDIT3, m_Mode);
	DDX_Text(pDX, IDC_EDIT2, m_Exp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModeDlg, CDialog)
	//{{AFX_MSG_MAP(CModeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModeDlg message handlers

void CModeDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CModeDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_Base="";
	m_Exp="";
	m_Mode="";
	CDialog::OnCancel();
}
