//Download by http://www.NewXing.com
// TestBignumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECDSA.h"
#include "TestBignumDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestBignumDlg dialog


CTestBignumDlg::CTestBignumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestBignumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestBignumDlg)
	m_BigNum1 = 0;
	m_BigNum2 = 0;
	//}}AFX_DATA_INIT
}


void CTestBignumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestBignumDlg)
	DDX_Text(pDX, IDC_EDIT1, m_BigNum1);
	DDV_MinMaxInt(pDX, m_BigNum1, 1, 65535);
	DDX_Text(pDX, IDC_EDIT2, m_BigNum2);
	DDV_MinMaxInt(pDX, m_BigNum2, 1, 65536);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestBignumDlg, CDialog)
	//{{AFX_MSG_MAP(CTestBignumDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestBignumDlg message handlers
