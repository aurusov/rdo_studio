#include "stdafx.h"
#include "rdoprocess_projectbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPProjectBar
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RPProjectBar, RPDockWnd)
	//{{AFX_MSG_MAP(RPProjectBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RPProjectBar::RPProjectBar()
{
}

RPProjectBar::~RPProjectBar()
{
}

int RPProjectBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if ( RPDockWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	SetBarStyle( GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

	pagectrl.Create( "", "", 0, CRect(0,0,0,0), this, -1 );
	pagectrl.ShowWindow( SW_SHOW );

	return 0;
}
