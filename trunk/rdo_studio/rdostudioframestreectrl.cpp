#include "stdafx.h"
#include "rdostudioframestreectrl.h"
#include "resource.h"

using namespace std;

// ----------------------------------------------------------------------------
// ---------- RDOStudioFramesTreeCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioFramesTreeCtrl, RDOTreeCtrl)
	//{{AFX_MSG_MAP(RDOStudioFramesTreeCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOStudioFramesTreeCtrl::RDOStudioFramesTreeCtrl()
{
}

RDOStudioFramesTreeCtrl::~RDOStudioFramesTreeCtrl()
{
}

BOOL RDOStudioFramesTreeCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if ( !RDOTreeCtrl::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_TABSTOP | TVS_HASLINES | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return TRUE;
}

int RDOStudioFramesTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( RDOTreeCtrl::OnCreate(lpCreateStruct) == -1 ) return -1;

	imageList.Create( 16, 16, ILC_COLORDDB | ILC_MASK, 5, 1 );
	CBitmap bmp;
	bmp.LoadBitmap( IDB_FRAMESTREECTRL );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
	SetImageList( &imageList, TVSIL_NORMAL );
	InsertItem( "Frames", 0, 0 );

	return 0;
}

void RDOStudioFramesTreeCtrl::insertItem( const string& name )
{
	InsertItem( name.c_str(), 1, 1, GetRootItem() );
}

void RDOStudioFramesTreeCtrl::clear()
{
	deleteChildren( GetRootItem() );
}

void RDOStudioFramesTreeCtrl::expand()
{
	Expand( GetRootItem(), TVE_EXPAND );
}
