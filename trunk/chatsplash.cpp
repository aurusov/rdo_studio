#include "stdafx.h"
#include "chatsplash.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatSplash
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatSplash, CWnd )
	//{{AFX_MSG_MAP(CChatSplash)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatSplash::CChatSplash():
	CWnd(),
	initInfo( "" ),
	useProgress( false )
{
}

CChatSplash::~CChatSplash()
{
}

void CChatSplash::show( CWnd* parent )
{
	CreateEx( 0, NULL, NULL, 0, 0, 0, 300, 95, parent->GetSafeHwnd(), NULL );
	update();
}

BOOL CChatSplash::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style     = WS_POPUP | WS_VISIBLE | WS_CAPTION;
	cs.dwExStyle = WS_EX_TOOLWINDOW;
	cs.lpszClass = AfxRegisterWndClass( 0, ::LoadCursor(NULL, IDC_ARROW) );
	if( !CWnd::PreCreateWindow( cs ) ) return FALSE;
	return TRUE;
}

int CChatSplash::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ( CWnd::OnCreate(lpCreateStruct) == -1 ) return -1;

	HFONT hf = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	bool flag = false;
	if ( hf ) {
		CFont* f = CFont::FromHandle( hf );
		if ( f ) {
			LOGFONT lf;
			f->GetLogFont( &lf );
			flag = font.CreateFontIndirect( &lf ) ? true : false;
		}
	}
	if ( !flag ) {
		LOGFONT lf;
		GetFont()->GetLogFont( &lf );
		font.CreateFontIndirect( &lf );
	}

	SetWindowText( format( IDS_SPLASH_RUNNING ).c_str() );

	CenterWindow();

	progress.Create( WS_CHILD | PBS_SMOOTH | WS_VISIBLE, CRect( 0, 0, 0, 0 ), this, 1 );

	return 0;
}

void CChatSplash::update()
{
	InvalidateRect( NULL );
	UpdateWindow();
}

void CChatSplash::setInitInfo( const std::string& str )
{
	initInfo = str;
	update();
}

void CChatSplash::setInitInfo( const UINT resID )
{
	setInitInfo( format( resID ) );
}

void CChatSplash::setProgress( const int min, const int max, const int step )
{
	progress.SetRange32( min, max );
	progress.SetStep( step );
	update();
}

void CChatSplash::stepProgress()
{
	progress.StepIt();
	useProgress = true;
	update();
}

void CChatSplash::hideProgress()
{
	useProgress = false;
	update();
}

void CChatSplash::OnPaint()
{
	CPaintDC dc( this );

	CRect clientRect;
	GetClientRect( clientRect );
	COLORREF bgcolor = ::GetSysColor( COLOR_BTNFACE );
	dc.FillSolidRect( clientRect, bgcolor );

	CRect textRect( 0, 0, 10, 1 );
	if ( !initInfo.empty() ) {
		CFont* prev_font = dc.SelectObject( &font );
		dc.SetBkMode( TRANSPARENT );
		dc.SetTextColor( ::GetSysColor( COLOR_WINDOWTEXT ) );
		std::string info = format( IDS_SPLASH_INITINFO, initInfo.c_str() );
		dc.DrawText( info.c_str(), -1, textRect, DT_SINGLELINE | DT_LEFT | DT_TOP | DT_CALCRECT );
		textRect.OffsetRect( 10, 10 );
		dc.DrawText( info.c_str(), -1, textRect, DT_SINGLELINE | DT_LEFT | DT_TOP );
		dc.SelectObject( prev_font );
	}
	if ( useProgress ) {
		progress.MoveWindow( clientRect.left + 4, textRect.bottom + 15, clientRect.Width() - 8, 20 );
	}
}
