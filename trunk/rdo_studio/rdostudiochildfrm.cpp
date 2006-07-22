#include "stdafx.h"
#include "rdostudiochildfrm.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioChildFrame
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOStudioChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(RDOStudioChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(RDOStudioChildFrame)
	//}}AFX_MSG_MAP
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
	if ( studioApp.mainFrame->OnCmdMsgForDockOnly( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	if ( CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) ) return TRUE;
	return FALSE;
}
