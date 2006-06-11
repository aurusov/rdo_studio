// rdoguiDoc.h : interface of the CRdoguiDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RDOGUIDOC_H__08683AAA_BDFE_46AC_8BB0_28DFA1EB73B5__INCLUDED_)
#define AFX_RDOGUIDOC_H__08683AAA_BDFE_46AC_8BB0_28DFA1EB73B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "rdosimcommon.h"

class CRdoguiDoc : public CDocument
{
protected: // create from serialization only
	CRdoguiDoc();
	DECLARE_DYNCREATE(CRdoguiDoc)

// Attributes
public:
	RdoModel *model;
	RDOFrame *frame;
	CMutex frameUsed;

	vector<int> scanCodes;
	vector<string> areasActivated;

	CWinThread* th;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRdoguiDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRdoguiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRdoguiDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOGUIDOC_H__08683AAA_BDFE_46AC_8BB0_28DFA1EB73B5__INCLUDED_)
