#include "stdafx.h"
#include "rdoprocess_dockwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCDockWnd
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOPROCDockWnd, CSizingControlBarG)
	//{{AFX_MSG_MAP(RDOPROCDockWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOPROCDockWnd::RDOPROCDockWnd()
{
}

RDOPROCDockWnd::~RDOPROCDockWnd()
{
}

int RDOPROCDockWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetSCBStyle( GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD );
	
	return 0;
}

BOOL RDOPROCDockWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !CSizingControlBarG::PreCreateWindow(cs) ) return FALSE;
//	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP;
//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
//	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) );
	return TRUE;
}
