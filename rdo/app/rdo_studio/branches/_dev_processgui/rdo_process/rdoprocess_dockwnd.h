#ifndef RDO_PROCESS_DOCKWND_H
#define RDO_PROCESS_DOCKWND_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "thirdparty/sizecbar/scbarg.h"

// --------------------------------------------------------------------------------
// -------------------- RPDockWnd
// --------------------------------------------------------------------------------
class RPDockWnd: public CSizingControlBarG
{
public:
	RPDockWnd();
	virtual ~RPDockWnd();

protected:
	//{{AFX_VIRTUAL(RPDockWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RPDockWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDO_PROCESS_DOCKWND_H
