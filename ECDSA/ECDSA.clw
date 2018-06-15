; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTestDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ECDSA.h"
LastPage=0

ClassCount=11
Class1=CECDSAApp
Class2=CECDSADoc
Class3=CECDSAView
Class4=CMainFrame

ResourceCount=8
Resource1=IDD_DIALOG_MINGWEN
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX
Class6=CECCDlg
Resource3=IDD_DIALOG_ECC
Class7=CMINGWENDlg
Resource4=IDD_DIALOG_BIGNUM
Class8=CTestDlg
Resource5=IDD_DIALOG_TEST
Class9=CTestBignumDlg
Resource6=IDD_DIALOG_MODE
Class10=CModeDlg
Resource7=IDR_MAINFRAME
Class11=CWeiShuDlg
Resource8=IDD_DIALOG_WEISHU

[CLS:CECDSAApp]
Type=0
HeaderFile=ECDSA.h
ImplementationFile=ECDSA.cpp
Filter=N

[CLS:CECDSADoc]
Type=0
HeaderFile=ECDSADoc.h
ImplementationFile=ECDSADoc.cpp
Filter=N

[CLS:CECDSAView]
Type=0
HeaderFile=ECDSAView.h
ImplementationFile=ECDSAView.cpp
Filter=C
BaseClass=CEditView
VirtualFilter=VWC
LastObject=ID_APP_EXIT


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_APP_EXIT




[CLS:CAboutDlg]
Type=0
HeaderFile=ECDSA.cpp
ImplementationFile=ECDSA.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_MENU_ECC
Command17=ID_MENU_BASEPOINT
Command18=ID_MENU_KEY
Command19=ID_MENU_SIGNATURE
Command20=ID_MENU_AUTHENTIC
Command21=ID_MENU_TEST
Command22=ID_APP_ABOUT
CommandCount=22

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_ECC
Command9=ID_BASE
Command10=ID_KEY
Command11=ID_SIGN
Command12=AUTHENTIC
Command13=ID_APP_ABOUT
CommandCount=13

[DLG:IDD_DIALOG_ECC]
Type=1
Class=CECCDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_Prime,edit,1350631552

[CLS:CECCDlg]
Type=0
HeaderFile=ECCDlg.h
ImplementationFile=ECCDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CECCDlg

[DLG:IDD_DIALOG_MINGWEN]
Type=1
Class=CMINGWENDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_MINGWEN,edit,1353781444
Control4=IDC_STATIC,static,1342308352

[CLS:CMINGWENDlg]
Type=0
HeaderFile=MINGWENDlg.h
ImplementationFile=MINGWENDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDCANCEL

[DLG:IDD_DIALOG_TEST]
Type=1
Class=CTestDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_RADIO1,button,1342308361
Control4=IDC_RADIO2,button,1342177289
Control5=IDC_RADIO3,button,1342177289
Control6=IDC_RADIO4,button,1342177289
Control7=IDC_RADIO5,button,1342177289
Control8=IDC_RADIO6,button,1342177289
Control9=IDC_RADIO7,button,1342177289
Control10=IDC_STATIC,button,1342177287

[CLS:CTestDlg]
Type=0
HeaderFile=TestDlg.h
ImplementationFile=TestDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_RADIO1
VirtualFilter=dWC

[DLG:IDD_DIALOG_BIGNUM]
Type=1
Class=CTestBignumDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_EDIT2,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:CTestBignumDlg]
Type=0
HeaderFile=TestBignumDlg.h
ImplementationFile=TestBignumDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_DIALOG_MODE]
Type=1
Class=CModeDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT1,edit,1350631552
Control7=IDC_EDIT2,edit,1350631552
Control8=IDC_EDIT3,edit,1350631552

[CLS:CModeDlg]
Type=0
HeaderFile=ModeDlg.h
ImplementationFile=ModeDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[CLS:CWeiShuDlg]
Type=0
HeaderFile=WeiShuDlg.h
ImplementationFile=WeiShuDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CWeiShuDlg

[DLG:IDD_DIALOG_WEISHU]
Type=1
Class=CWeiShuDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

