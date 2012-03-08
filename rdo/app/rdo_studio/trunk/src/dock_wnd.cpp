/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      dock_wnd.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/dock_wnd.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioDockWnd
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioDockWnd, CSizingControlBarG)
	ON_WM_CREATE()
END_MESSAGE_MAP()

RDOStudioDockWnd::RDOStudioDockWnd()
{
}

RDOStudioDockWnd::~RDOStudioDockWnd()
{
}

int RDOStudioDockWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetSCBStyle( GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD );
	
	return 0;
}

BOOL RDOStudioDockWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !CSizingControlBarG::PreCreateWindow(cs) ) return FALSE;
//	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP;
//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
//	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) );
	return TRUE;
}
