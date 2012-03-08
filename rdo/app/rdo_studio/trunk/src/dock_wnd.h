/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      dock_wnd.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_DOCK_WND_H_
#define _RDO_STUDIO_MFC_DOCK_WND_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "thirdparty/sizecbar/sizecbar.h"
#include "thirdparty/sizecbar/scbarg.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioDockWnd
// --------------------------------------------------------------------------------
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

#endif // _RDO_STUDIO_MFC_DOCK_WND_H_
