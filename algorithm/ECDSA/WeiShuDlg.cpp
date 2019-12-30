//Download by http://www.NewXing.com
// WeiShuDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECDSA.h"
#include "WeiShuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeiShuDlg dialog


CWeiShuDlg::CWeiShuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWeiShuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWeiShuDlg)
	m_WeiShu = 0;
	//}}AFX_DATA_INIT
}


void CWeiShuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWeiShuDlg)
	DDX_Text(pDX, IDC_EDIT1, m_WeiShu);
	DDV_MinMaxInt(pDX, m_WeiShu, 0, 65536);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWeiShuDlg, CDialog)
	//{{AFX_MSG_MAP(CWeiShuDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeiShuDlg message handlers

void CWeiShuDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}

void CWeiShuDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_WeiShu=0;
	CDialog::OnCancel();
}
