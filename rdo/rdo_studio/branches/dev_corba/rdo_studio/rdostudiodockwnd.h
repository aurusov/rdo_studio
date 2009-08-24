#ifndef RDOSTUDIODOCKWND_H
#define RDOSTUDIODOCKWND_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "sizecbar/sizecbar.h"
#include "sizecbar/scbarg.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioDockWnd
// ----------------------------------------------------------------------------
class RDOStudioDockWnd: public CSizingControlBarG
{
public:
	RDOStudioDockWnd();
	virtual ~RDOStudioDockWnd();

protected:
	//{{AFX_VIRTUAL(RDOStudioDockWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioDockWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIODOCKWND_H
