#include "stdafx.h"
#include "chatdockwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatDockWnd
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CChatDockWnd, CSizingControlBarG)
	//{{AFX_MSG_MAP(CChatDockWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatDockWnd::CChatDockWnd()
{
}

CChatDockWnd::~CChatDockWnd()
{
}

int CChatDockWnd::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	if ( CSizingControlBarG::OnCreate(lpCreateStruct) == -1 ) return -1;

	SetSCBStyle( GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD );

//	frames->Create( 0, CRect(0, 0, 0, 0), &tab, 0 );
	tree.Create( 0, CRect(0, 0, 0, 0), this, 0 );

	return 0;
}

BOOL CChatDockWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !CSizingControlBarG::PreCreateWindow(cs) ) return FALSE;
//	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP;
//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
//	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) );
	return TRUE;
}
