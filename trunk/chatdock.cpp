#include "stdafx.h"
#include "chatdock.h"
#include "chatapp.h"
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
	images.Create( IDB_DOCK, 16, 0, RGB( 0xFF, 0x00, 0xFF ) );
	static_cast<CTabCtrl*>(tab.getTabAsParent())->SetImageList( &images );
	tab.setAutoDelete( false );

	m_szVert.cx = 220;

	users.Create( 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 1 );
	network.Create( 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 2 );
	if ( chatApp.isWinNT() ) {
		smiles.Create( NULL, NULL, 0, CRect(0, 0, 0, 0), tab.getTabAsParent(), 3 );
	}

	tab.insertItem( &users, format( IDS_USERS ).c_str(), 0 );
	tab.insertItem( &network, format( IDS_NETWORK ).c_str(), 1 );
	if ( chatApp.isWinNT() ) {
		tab.insertItem( &smiles, format( IDS_SMILES ).c_str(), 2 );
	}

	return 0;
}

BOOL CChatDock::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if ( tab.OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) ) return TRUE;
	return CSizingControlBarG::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}
