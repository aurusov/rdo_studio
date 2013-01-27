/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/child_frm.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/child_frm.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_windows_base.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioChildFrame
// --------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(RDOStudioChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()

RDOStudioChildFrame::RDOStudioChildFrame(): CMDIChildWnd()
{
}

RDOStudioChildFrame::~RDOStudioChildFrame()
{
}

BOOL RDOStudioChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if ( !CMDIChildWnd::PreCreateWindow(cs) ) return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	return TRUE;
}

#ifdef _DEBUG
void RDOStudioChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void RDOStudioChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif

BOOL RDOStudioChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
//	if ( studioApp.m_pMainFrame->OnCmdMsgForDockOnly( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	if ( CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ) return TRUE;
	return FALSE;
}
