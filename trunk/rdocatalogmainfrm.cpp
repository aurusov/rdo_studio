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
	ON_UPDATE_COMMAND_UI( ID_URL_STATUSBAR, OnUpdateUrlStatusBar )
END_MESSAGE_MAP()

static UINT indicators[] = {
	ID_EMPTY_STATUSBAR,
	ID_URL_STATUSBAR
};

RDOCatalogMainFrame::RDOCatalogMainFrame():
	CFrameWnd(),
	url( "" )
{
}

RDOCatalogMainFrame::~RDOCatalogMainFrame()
{
}

int RDOCatalogMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CFrameWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	statusBar.Create( this );
	statusBar.SetIndicators( indicators, sizeof(indicators)/sizeof(UINT) );
	statusBar.SetPaneInfo( 0, ID_EMPTY_STATUSBAR, SBPS_NORMAL | SBPS_NOBORDERS, 0 );
	statusBar.SetPaneInfo( 1, ID_URL_STATUSBAR  , SBPS_STRETCH | SBPS_NOBORDERS, 70 );

	return 0;
}

BOOL RDOCatalogMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CFrameWnd::PreCreateWindow(cs) ) return FALSE;
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}

void RDOCatalogMainFrame::setUrl( const CString& _url )
{
	url = _url;
}

void RDOCatalogMainFrame::OnUpdateUrlStatusBar( CCmdUI *pCmdUI )
{
	pCmdUI->Enable();
	pCmdUI->SetText( url );
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
