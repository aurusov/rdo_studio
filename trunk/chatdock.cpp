#include "stdafx.h"
#include "chatdock.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatDock
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CChatDock, CSizingControlBarG)
	//{{AFX_MSG_MAP(CChatDock)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatDock::CChatDock()
{
}

CChatDock::~CChatDock()
{
}

int CChatDock::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	if ( CSizingControlBarG::OnCreate(lpCreateStruct) == -1 ) return -1;

	SetSCBStyle( GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD );

	tab.Create( NULL, NULL, 0, CRect(0, 0, 100, 100), this, 0 );
	tab.modifyTabStyle( 0, TCS_BOTTOM | TCS_MULTILINE );
	tab.setAutoDelete( false );

	users.Create( 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );
	network.Create( 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 0 );

	tab.insertItem( &users, format( IDS_USERS ).c_str() );
	tab.insertItem( &network, format( IDS_NETWORK ).c_str() );

	return 0;
}
