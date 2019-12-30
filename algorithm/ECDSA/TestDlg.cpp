//Download by http://www.NewXing.com
// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ECDSA.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog


CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	pCWnd=pParent;
	flag=0;
	//}}AFX_DATA_INIT
}


void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_RADIO1, m_RadioMultiply);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

void CTestDlg::OnOK() 
{
	// TODO: Add extra validation hereCheckRadioButton(IDC_RADIO1,IDC_RADIO7,IDC_RADIO1);
	int iRadio;
	iRadio=GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO7);
//	if(iMaleRadio==IDC_MALE_RADIO)
	switch(iRadio)
	{
	case IDC_RADIO1:
		flag=1;
		break;
	case IDC_RADIO2:
		flag=2;
		break;
	case IDC_RADIO3:
		flag=3;
		break;
	case IDC_RADIO4:
		flag=4;
		break;
	case IDC_RADIO5:
		flag=5;
		break;
	case IDC_RADIO6:
		flag=6;
		break;
	case IDC_RADIO7:
		flag=7;
		break;
	default:
		MessageBox("没有选择测试项目");return;
	}
		
	CDialog::OnOK();
}

void CTestDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	flag=0;
	CDialog::OnCancel();
}
