#include "stdafx.h"
#include "chatnetlistctrl.h"
//#include "chatapp.h"
//#include "chatmainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatNetListCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatNetListCtrl, RDOTreeCtrl )
	//{{AFX_MSG_MAP(CChatNetListCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatNetListCtrl::CChatNetListCtrl(): RDOTreeCtrl()
{
}

CChatNetListCtrl::~CChatNetListCtrl()
{
}

BOOL CChatNetListCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !RDOTreeCtrl::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return TRUE;
}

int CChatNetListCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
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
