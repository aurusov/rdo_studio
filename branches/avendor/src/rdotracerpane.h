#if !defined(AFX_RDOTRACERPANE_H__334DF087_06C2_4500_89EB_4C6BC74E6E43__INCLUDED_)
#define AFX_RDOTRACERPANE_H__334DF087_06C2_4500_89EB_4C6BC74E6E43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// rdotracerpane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RDOTracerPane window

class RDOTracerPane : public CWnd
{
DECLARE_DYNCREATE( RDOTracerPane )
protected:
	CWnd* control;
	void resizeControl();
public:
	RDOTracerPane();

public:

	//{{AFX_VIRTUAL(RDOTracerPane)
	//}}AFX_VIRTUAL

public:
	virtual ~RDOTracerPane();

	const CWnd* getControl() const { return control; };
	void setControl( CWnd* _control );

protected:
	//{{AFX_MSG(RDOTracerPane)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RDOTRACERPANE_H__334DF087_06C2_4500_89EB_4C6BC74E6E43__INCLUDED_)
