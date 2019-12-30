//Download by http://www.NewXing.com
// MINGWENDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECDSA.h"
#include "MINGWENDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMINGWENDlg dialog


CMINGWENDlg::CMINGWENDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMINGWENDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMINGWENDlg)
	m_MingWen =_T("Chengdu University of\r\nTechnology");
	//}}AFX_DATA_INIT
}


void CMINGWENDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMINGWENDlg)
	DDX_Text(pDX, IDC_EDIT_MINGWEN, m_MingWen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMINGWENDlg, CDialog)
	//{{AFX_MSG_MAP(CMINGWENDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMINGWENDlg message handlers

void CMINGWENDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_MingWen="";//¸³ÖµÎª¿Õ
	CDialog::OnCancel();
}

void CMINGWENDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}
