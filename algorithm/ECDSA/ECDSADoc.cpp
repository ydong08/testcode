//Download by http://www.NewXing.com
// ECDSADoc.cpp : implementation of the CECDSADoc class
//

#include "stdafx.h"
#include "ECDSA.h"

#include "ECDSADoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECDSADoc

IMPLEMENT_DYNCREATE(CECDSADoc, CDocument)

BEGIN_MESSAGE_MAP(CECDSADoc, CDocument)
	//{{AFX_MSG_MAP(CECDSADoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECDSADoc construction/destruction

CECDSADoc::CECDSADoc()
{
	// TODO: add one-time construction code here

}

CECDSADoc::~CECDSADoc()
{
}

BOOL CECDSADoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CECDSADoc serialization

void CECDSADoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CECDSADoc diagnostics

#ifdef _DEBUG
void CECDSADoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CECDSADoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CECDSADoc commands
