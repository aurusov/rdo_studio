#include "rdo_ctrls_pch.h"
#include "rdolink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOLink
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOLink, CWnd )
	//{{AFX_MSG_MAP(RDOLink)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOLink::RDOLink():
	CWnd(),
	mustInit( true ),
	bold( false ),
	useTextColor( false ),
	useBgColor( false ),
	isMouseCaptionOver( false )
{
	BYTE andData[] = {
		0xF9, 0xFF, 0xFF, 0xFF,
		0xF0, 0xFF, 0xFF, 0xFF,
		0xF0, 0xFF, 0xFF, 0xFF,
		0xF0, 0xFF, 0xFF, 0xFF,
		0xF0, 0xFF, 0xFF, 0xFF,
		0xF0, 0x3F, 0xFF, 0xFF,
		0xF0, 0x07, 0xFF, 0xFF,
		0xF0, 0x01, 0xFF, 0xFF,
		0xF0, 0x00, 0xFF, 0xFF,
		0x10, 0x00, 0x7F, 0xFF,
		0x00, 0x00, 0x7F, 0xFF,
		0x00, 0x00, 0x7F, 0xFF,
		0x80, 0x00, 0x7F, 0xFF,
		0xC0, 0x00, 0x7F, 0xFF,
		0xC0, 0x00, 0x7F, 0xFF,
		0xE0, 0x00, 0x7F, 0xFF,
		0xE0, 0x00, 0xFF, 0xFF,
		0xF0, 0x00, 0xFF, 0xFF,
		0xF0, 0x00, 0xFF, 0xFF,
		0xF8, 0x01, 0xFF, 0xFF,
		0xF8, 0x01, 0xFF, 0xFF,
		0xF8, 0x01, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF
	};

	BYTE xorData[] = {
		0x00, 0x00, 0x00, 0x00,
		0x06, 0x00, 0x00, 0x00,
		0x06, 0x00, 0x00, 0x00,
		0x06, 0x00, 0x00, 0x00,
		0x06, 0x00, 0x00, 0x00,
		0x06, 0x00, 0x00, 0x00,
		0x06, 0xC0, 0x00, 0x00,
		0x06, 0xD8, 0x00, 0x00,
		0x06, 0xDA, 0x00, 0x00,
		0x06, 0xDB, 0x00, 0x00,
		0x67, 0xFB, 0x00, 0x00,
		0x77, 0xFF, 0x00, 0x00,
		0x37, 0xFF, 0x00, 0x00,
		0x17, 0xFF, 0x00, 0x00,
		0x1F, 0xFF, 0x00, 0x00,
		0x0F, 0xFF, 0x00, 0x00,
		0x0F, 0xFE, 0x00, 0x00,
		0x07, 0xFE, 0x00, 0x00,
		0x07, 0xFE, 0x00, 0x00,
		0x03, 0xFC, 0x00, 0x00,
		0x03, 0xFC, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	};

	cursor = CreateCursor( AfxGetApp()->m_hInstance, 7, 1, 32, 32, andData, xorData );
}

RDOLink::~RDOLink()
{
	if ( cursor ) ::DestroyCursor( cursor );
}

BOOL RDOLink::Create( const CString& text, const RECT& rect, CWnd* pParentWnd, UINT nID )
{
	return CWnd::Create( "STATIC", text, WS_CHILD | WS_VISIBLE | SS_NOTIFY, rect, pParentWnd, nID );
}

void RDOLink::setTextColor( const COLORREF color )
{
	textColor    = color;
	useTextColor = true;
}

void RDOLink::setBgColor( const COLORREF color )
{
	bgColor    = color;
	useBgColor = true;
}

void RDOLink::setFontBold( const bool value )
{
	bold = value;
}

void RDOLink::OnLButtonDown( UINT nFlags, CPoint point )
{
	::ReleaseCapture();

	CWnd::OnLButtonDown( nFlags, point );

	if ( GetParent() ) {
		NMHDR NotifyStruct;
		NotifyStruct.hwndFrom = m_hWnd;
		NotifyStruct.code     = NM_RDOLINK_CLICK;
		::SendMessage( GetParent()->m_hWnd, WM_NOTIFY, 0, (unsigned int)&NotifyStruct );
	}

	isMouseCaptionOver = false;
	SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
	RedrawWindow();
}

void RDOLink::OnMouseMove( UINT nFlags, CPoint point )
{
	CWnd::OnMouseMove( nFlags, point );
	CDC* dc = GetDC();
	CFont* prevFont = dc->SelectObject( &fontNormal );

	CRect r;
	GetClientRect( r );
	CRect rectCaption = r;

	CString s;
	GetWindowText( s );
	dc->DrawText( s, rectCaption, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_CALCRECT );
	r.DeflateRect( (r.Width() - rectCaption.Width())/2, (r.Height() - rectCaption.Height())/2 );
	bool isMouseCaptionOverPrev = isMouseCaptionOver;
	isMouseCaptionOver = r.PtInRect( point ) ? true : false;
	if ( isMouseCaptionOverPrev != isMouseCaptionOver ) {
		if ( isMouseCaptionOver ) {
			SetCursor( cursor );
			SetCapture();
		} else {
			::ReleaseCapture();
			SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
		}
		RedrawWindow();
	}

	dc->SelectObject( prevFont );
	ReleaseDC( dc );
}

void RDOLink::OnPaint()
{
	CPaintDC dc( this );
	CRect r;
	GetClientRect( r );

	if ( mustInit ) init();

	int saveIndex = dc.SaveDC();
	if ( isMouseCaptionOver ) {
		dc.SelectObject( &fontOver );
	} else {
		dc.SelectObject( &fontNormal );
	}

	dc.FillSolidRect( r, useBgColor ? bgColor : GetSysColor(COLOR_BTNFACE) );
	dc.SetBkMode( TRANSPARENT );
	dc.SetTextColor( useTextColor ? textColor : GetSysColor(COLOR_WINDOWTEXT) );
	CString s;
	GetWindowText( s );
	dc.DrawText( s, r, DT_SINGLELINE | DT_CENTER | DT_VCENTER );

	dc.RestoreDC( saveIndex );
}

void RDOLink::OnKillFocus( CWnd* pNewWnd )
{
	CWnd::OnKillFocus( pNewWnd );

	::ReleaseCapture();
	isMouseCaptionOver = false;
	SetCursor( ::LoadCursor( NULL, IDC_ARROW ) );
	RedrawWindow();
}

void RDOLink::init()
{
	if ( mustInit ) {
		LOGFONT lf;
		CWnd* parent = GetParent();
		if ( parent ) {
			parent->GetFont()->GetLogFont( &lf );
		} else {
			HFONT hf = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
			CFont* f = CFont::FromHandle( hf );
			f->GetLogFont( &lf );
		}
		if ( bold ) lf.lfWeight = FW_BOLD;
		fontNormal.CreateFontIndirect( &lf );
		lf.lfUnderline = true;
		fontOver.CreateFontIndirect( &lf );

		mustInit = false;
	}
}

CSize RDOLink::calcSize()
{
	if ( mustInit ) init();

	CRect r( 0, 0, 1, 1 );
	CString s;
	GetWindowText( s );

	CDC* dc = GetDC();
	CFont* prevFont = dc->SelectObject( &fontOver );
	dc->DrawText( s, r, DT_SINGLELINE | DT_CALCRECT );
	dc->SelectObject( prevFont );
	ReleaseDC( dc );
	return CSize( r.Width(), r.Height() );
}

void RDOLink::autoSize()
{
	CSize size = calcSize();

	CRect r;
	GetWindowRect( r );
	ScreenToClient( r );
	r.right  = r.left + size.cx;
	r.bottom = r.top  + size.cy;
	MoveWindow( r );
}
