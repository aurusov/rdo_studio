// rdotracerpane.cpp : implementation file
//

#include "stdafx.h"
#include "rdotracerpane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RDOTracerPane

RDOTracerPane::RDOTracerPane()
	: CWnd(),
	control( NULL )
{
}

RDOTracerPane::~RDOTracerPane()
{
}

IMPLEMENT_DYNCREATE( RDOTracerPane, CWnd )

BEGIN_MESSAGE_MAP(RDOTracerPane, CWnd)
	//{{AFX_MSG_MAP(RDOTracerPane)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// RDOTracerPane message handlers

void RDOTracerPane::resizeControl()
{
	if ( !control ) return;
	CRect rect;
	GetClientRect( &rect );
	control->MoveWindow( rect, FALSE );
}

void RDOTracerPane::setControl( CWnd* _control )
{
	if ( control != _control ) {
		control = _control;
		LONG control_style = ::GetWindowLong( control->GetSafeHwnd(), GWL_EXSTYLE );
		if ( control_style & WS_EX_CLIENTEDGE ) {
			control_style ^= WS_EX_CLIENTEDGE;
			::SetWindowLong( control->GetSafeHwnd(), GWL_EXSTYLE, control_style );
		}
		resizeControl();
	}
}

void RDOTracerPane::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	resizeControl();	
}
