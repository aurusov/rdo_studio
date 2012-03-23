/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_dockwnd.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Шувалов Андрей (dronbas@gmail.com)
  \date      20.02.2006
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_PROCESS_DOCKWND_H_
#define _RDO_STUDIO_MFC_RDO_PROCESS_DOCKWND_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "thirdparty/sizecbar/scbarg.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RPDockWnd
// --------------------------------------------------------------------------------
class RPDockWnd: public CSizingControlBarG
{
public:
	RPDockWnd();
	virtual ~RPDockWnd();

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_RDO_PROCESS_DOCKWND_H_
