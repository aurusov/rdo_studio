#include "stdafx.h"
#include "chatnetlistdock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatNetListDock
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CChatNetListDock, CSizingControlBarG)
	//{{AFX_MSG_MAP(CChatNetListDock)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatNetListDock::CChatNetListDock()
{
}

CChatNetListDock::~CChatNetListDock()
{
}

int CChatNetListDock::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	if ( CSizingControlBarG::OnCreate(lpCreateStruct) == -1 ) return -1;

	SetSCBStyle( GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD );

	list.Create( 0, CRect(0, 0, 0, 0), this, 0 );

	return 0;
}
