#include "stdafx.h"
#include "chatnetworkdock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatNetworkDock
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CChatNetworkDock, CSizingControlBarG)
	//{{AFX_MSG_MAP(CChatNetworkDock)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatNetworkDock::CChatNetworkDock()
{
}

CChatNetworkDock::~CChatNetworkDock()
{
}

int CChatNetworkDock::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	if ( CSizingControlBarG::OnCreate(lpCreateStruct) == -1 ) return -1;

	SetSCBStyle( GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD );

	list.Create( 0, CRect(0, 0, 0, 0), this, 0 );

	return 0;
}
