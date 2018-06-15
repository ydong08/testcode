//Download by http://www.NewXing.com
// ECCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECDSA.h"
#include "ECCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECCDlg dialog


CECCDlg::CECCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CECCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CECCDlg)
	m_PrimeNum = _T("2");
	pCWnd=pParent;
	//}}AFX_DATA_INIT
}


void CECCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CECCDlg)
	DDX_Text(pDX, IDC_EDIT_Prime, m_PrimeNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CECCDlg, CDialog)
	//{{AFX_MSG_MAP(CECCDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECCDlg message handlers

void CECCDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);//Ë¢ÐÂ±äÁ¿
	CDialog::OnOK();
}

void CECCDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_PrimeNum=_T("0");
	CDialog::OnCancel();
}
