#include "stdafx.h"
#include "chatuserstreectrl.h"
#include "chatapp.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatUsersTreeCtrl
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE( CChatUsersTreeCtrl, RDOTreeCtrl )

BEGIN_MESSAGE_MAP( CChatUsersTreeCtrl, RDOTreeCtrl )
	//{{AFX_MSG_MAP(CChatUsersTreeCtrl)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatUsersTreeCtrl::CChatUsersTreeCtrl(): RDOTreeCtrl()
{
}

CChatUsersTreeCtrl::~CChatUsersTreeCtrl()
{
}

BOOL CChatUsersTreeCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !RDOTreeCtrl::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return TRUE;
}

BOOL CChatUsersTreeCtrl::DestroyWindow()
{
	return RDOTreeCtrl::DestroyWindow();
}

int CChatUsersTreeCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( RDOTreeCtrl::OnCreate( lpCreateStruct ) == -1 ) return -1;

//	ModifyStyle( 0, WS_TABSTOP | TVS_HASBUTTONS /*| TVS_LINESATROOT*/ | TVS_HASLINES | TVS_DISABLEDRAGDROP );

	imageList.Create( 16, 16, ILC_COLORDDB | ILC_MASK, 3, 1 );
	CBitmap bmp;
	bmp.LoadBitmap( IDB_STATUSMODE );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
	SetImageList( &imageList, TVSIL_NORMAL );

	return 0;
}

HTREEITEM CChatUsersTreeCtrl::findUser( const CChatUser* const user )
{
	HTREEITEM item = GetRootItem();

	while ( item ) {
		CChatUser* data = (CChatUser*)GetItemData( item );
		if ( data == user ) return item;
		item = GetNextItem( item, TVGN_NEXT );
	}

	return NULL;
}

void CChatUsersTreeCtrl::addUser( const CChatUser* const user )
{
	if ( !findUser( user ) ) {
		HTREEITEM item;
		item = InsertItem( user->getUserName(), user->getStatusMode(), user->getStatusMode() );
		SetItemData( item, (DWORD)user );
		SortChildren( TVI_ROOT );
	}
}

void CChatUsersTreeCtrl::deleteUser( const CChatUser* const user )
{
	HTREEITEM item;
	item = findUser( user );
	if ( item ) {
		DeleteItem( item );
	}
}

void CChatUsersTreeCtrl::updateUserName( const CChatUser* const user )
{
	HTREEITEM item = findUser( user );
	if ( item ) {
		SetItemText( item, user->getUserName() );
	}
}

void CChatUsersTreeCtrl::updateUserStatus( const CChatUser* const user )
{
	HTREEITEM item = findUser( user );
	if ( item ) {
		SetItemImage( item, user->getStatusMode(), user->getStatusMode() );
	}
}

void CChatUsersTreeCtrl::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	RDOTreeCtrl::OnKeyDown( nChar, nRepCnt, nFlags );

	chatApp.pFrame->restoreStatusMode();
}
