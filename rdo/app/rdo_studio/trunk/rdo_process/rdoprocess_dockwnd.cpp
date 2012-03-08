#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_dockwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPDockWnd
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RPDockWnd, CSizingControlBarG)
	ON_WM_CREATE()
END_MESSAGE_MAP()

RPDockWnd::RPDockWnd()
{
}

RPDockWnd::~RPDockWnd()
{
}

int RPDockWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetSCBStyle( GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD );
	
	return 0;
}

BOOL RPDockWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !CSizingControlBarG::PreCreateWindow(cs) ) return FALSE;
//	cs.style = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP;
//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
//	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) );
	return TRUE;
}
