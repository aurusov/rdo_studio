#include "stdafx.h"
#include "bkemulchildview.h"
#include "ddutil.h"

#include <ddraw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------
// ---------- BKChildView
// --------------------------------------------------------------
#define SAFE_DELETE( p )  { if( p ) { delete ( p );     ( p ) = NULL; } }
#define SAFE_RELEASE( p ) { if( p ) { ( p )->Release(); ( p ) = NULL; } }
const int bk_width  = 512;
const int bk_height = 256;

BEGIN_MESSAGE_MAP(BKChildView,CWnd )
	//{{AFX_MSG_MAP(BKChildView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BKChildView::BKChildView():
	CWnd(),
	display( NULL ),
	surface( NULL )
{
}

BKChildView::~BKChildView()
{
	if ( surface ) { delete surface; surface = NULL; }
	if ( display ) { delete display; display = NULL; }
}

BOOL BKChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if ( !CWnd::PreCreateWindow(cs) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) /*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );

	return TRUE;
}

int BKChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CWnd ::OnCreate(lpCreateStruct) == -1 ) return -1;

	initDirectDraw();

	return 0;
}

HRESULT BKChildView::initDirectDraw()
{
	display = new CDisplay();

	if ( display->CreateWindowedDisplay( this, bk_width, bk_height ) != S_OK ) {
		AfxMessageBox( "Failed initializing DirectDraw." );
		return -1;
	}

	if ( display->CreateSurface( &surface, bk_width, bk_height ) != S_OK ) {
		return -1;
	}

	return S_OK;
}

void BKChildView::OnPaint()
{
	CPaintDC dc( this );
	if ( displayFrame() == DDERR_SURFACELOST ) {
		restoreSurfaces();
		displayFrame();
	}
	CRect rect;
	GetClientRect( rect );
	if ( rect.Width() > bk_width ) {
		dc.FillSolidRect( bk_width, 0, rect.right - bk_width, rect.bottom, RGB( 0xFF, 0xFF, 0xFF ) );
	}
	if ( rect.Height() > bk_height ) {
		dc.FillSolidRect( 0, bk_height, bk_width, rect.bottom - bk_height, RGB( 0xFF, 0xFF, 0xFF ) );
	}
}

HRESULT BKChildView::displayFrame()
{
	display->Clear( 0 );
	display->Blt( 0, 0, surface, NULL );
	return display->Present();
}

HRESULT BKChildView::restoreSurfaces()
{
	return display->GetDirectDraw()->RestoreAllSurfaces();
}

void BKChildView::updateBounds()
{
	display->UpdateBounds();
	InvalidateRect( NULL );
	UpdateWindow();
}
