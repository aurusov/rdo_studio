#include "stdafx.h"
#include "rdocatalogmainfrm.h"
#include "rdocatalogapp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOCatalogMainFrame
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(RDOCatalogMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(RDOCatalogMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(RDOCatalogMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOCatalogMainFrame::RDOCatalogMainFrame(): CFrameWnd()
{
}

RDOCatalogMainFrame::~RDOCatalogMainFrame()
{
}

int RDOCatalogMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;
	return 0;
}

BOOL RDOCatalogMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CFrameWnd::PreCreateWindow(cs) ) return FALSE;
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}

#ifdef _DEBUG
void RDOCatalogMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void RDOCatalogMainFrame::Dump( CDumpContext& dc ) const
{
	CFrameWnd::Dump( dc );
}
#endif
