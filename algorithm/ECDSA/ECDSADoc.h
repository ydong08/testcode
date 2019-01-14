//Download by http://www.NewXing.com
// ECDSADoc.h : interface of the CECDSADoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECDSADOC_H__E67FDA94_E44A_48FA_9643_5B848BC84723__INCLUDED_)
#define AFX_ECDSADOC_H__E67FDA94_E44A_48FA_9643_5B848BC84723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CECDSADoc : public CDocument
{
protected: // create from serialization only
	CECDSADoc();
	DECLARE_DYNCREATE(CECDSADoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECDSADoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CECDSADoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CECDSADoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECDSADOC_H__E67FDA94_E44A_48FA_9643_5B848BC84723__INCLUDED_)
