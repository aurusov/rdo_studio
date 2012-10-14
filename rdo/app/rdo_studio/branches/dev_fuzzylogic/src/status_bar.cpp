/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      status_bar.cpp
  \author    Захаров Павел
  \date      09.04.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/status_bar.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioStatusBar
// --------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(RDOStudioStatusBar, CStatusBar)

BEGIN_MESSAGE_MAP(RDOStudioStatusBar, CStatusBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

RDOStudioStatusBar::RDOStudioStatusBar():
	CStatusBar(),
	visible( false ),
	indicator( 0 )
{
}

RDOStudioStatusBar::~RDOStudioStatusBar()
{
}

int RDOStudioStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CStatusBar::OnCreate(lpCreateStruct) == -1 ) return -1;

	progress.Create( WS_CHILD | PBS_SMOOTH, CRect( 0, 0, 0, 0 ), this, 1 );

	return 0;
}

void RDOStudioStatusBar::repositionProgressCtrl( const rbool redraw )
{
	CRect rect;
	GetItemRect( CommandToIndex( indicator ), rect );
	progress.MoveWindow( rect, redraw );
}

void RDOStudioStatusBar::setProgressVisible( const rbool _visible )
{
	if ( visible != _visible && GetSafeHwnd() ) {
		visible = _visible;
		if ( visible )
			repositionProgressCtrl();
		progress.ShowWindow( visible ? SW_SHOWNA : SW_HIDE );
	}
}

void RDOStudioStatusBar::OnSize(UINT nType, int cx, int cy) 
{
	CStatusBar::OnSize(nType, cx, cy);
	repositionProgressCtrl( visible );
}
