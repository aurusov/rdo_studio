#include "app/rdo_studio_mfc/pch/stdpch.h"
#include "app/rdo_studio_mfc/src/frame/options_ctrl.h"
#include "app/rdo_studio_mfc/src/frame/style.h"
#include "app/rdo_studio_mfc/src/application.h"
#include "app/rdo_studio_mfc/src/main_frm.h"
#include "app/rdo_studio_mfc/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameOptionsCtrl
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioFrameOptionsCtrl, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

RDOStudioFrameOptionsCtrl::RDOStudioFrameOptionsCtrl():
	CWnd(),
	style( NULL )
{
	bmp.LoadBitmap( IDB_FRAME_OPTIONS_CTRL );
}

RDOStudioFrameOptionsCtrl::~RDOStudioFrameOptionsCtrl()
{
	bmp.DeleteObject();
}

BOOL RDOStudioFrameOptionsCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CWnd::PreCreateWindow( cs ) ) return FALSE;

	cs.style &= ~WS_BORDER;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) );

	return TRUE;
}

#ifdef _DEBUG
void RDOStudioFrameOptionsCtrl::AssertValid() const
{
	CWnd::AssertValid();
}

void RDOStudioFrameOptionsCtrl::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif

void RDOStudioFrameOptionsCtrl::setStyle( RDOStudioFrameStyle* _style )
{
	style = _style;
	if ( font.m_hObject ) {
		font.DeleteObject();
	}
	LOGFONT lf;
	memset( &lf, 0, sizeof(lf) );
	CDC* dc = GetDC();
	lf.lfHeight    = -MulDiv( style->font->size, ::GetDeviceCaps( dc->m_hDC, LOGPIXELSY ), 72 );
	lf.lfWeight    = style->theme->defaultStyle & rdoStyle::RDOStyleFont::BOLD ? FW_BOLD : FW_NORMAL;
	lf.lfItalic    = style->theme->defaultStyle & rdoStyle::RDOStyleFont::ITALIC;
	lf.lfUnderline = style->theme->defaultStyle & rdoStyle::RDOStyleFont::UNDERLINE;
	lf.lfCharSet   = BYTE(style->font->characterSet);
#pragma warning(disable: 4996)
	strcpy( lf.lfFaceName, style->font->name.c_str() );
#pragma warning(default: 4996)
	font.CreateFontIndirect( &lf );
	ReleaseDC( dc );

	InvalidateRect( NULL );
	UpdateWindow();
}

void RDOStudioFrameOptionsCtrl::OnPaint()
{
	if ( style && font.m_hObject ) {

		CPaintDC dc( this );

		CDC bmpDC;
		bmpDC.CreateCompatibleDC( &dc );
		CBitmap* pOldBitmap = bmpDC.SelectObject( &bmp );
		BITMAP bmpInfo;
		bmp.GetBitmap( &bmpInfo );
		dc.BitBlt( 1, 1, bmpInfo.bmWidth, bmpInfo.bmHeight, &bmpDC, 0, 0, SRCCOPY );
		bmpDC.SelectObject( pOldBitmap );

		CFont* pOldFont = dc.SelectObject( &font );
		dc.SetBkMode( TRANSPARENT );
		dc.SetTextColor( RGB( 0xFF, 0xFF, 0 ) );
		std::string str = "Sample of drawing text";
		dc.DrawText( str.c_str(), str.length(), CRect( 3, 12, bmpInfo.bmWidth + 1, bmpInfo.bmHeight + 1 ), DT_LEFT );

		CPen pen( PS_SOLID, 0, style->theme->defaultColor );
		CPen* pOldPen = dc.SelectObject( &pen );

		CPoint points[5];
		points[0].x = 0;
		points[0].y = 0;
		points[1].x = bmpInfo.bmWidth + 1;
		points[1].y = 0;
		points[2].x = bmpInfo.bmWidth + 1;
		points[2].y = bmpInfo.bmHeight + 1;
		points[3].x = 0;
		points[3].y = bmpInfo.bmHeight + 1;
		points[4].x = 0;
		points[4].y = 0;
		dc.Polyline( points, 5 );

		CBrush brush( style->theme->backgroundColor );
		CBrush* pOldBrush = dc.SelectObject( &brush );

		CRect rect;
		GetClientRect( rect );
		dc.FillRect( CRect( bmpInfo.bmWidth + 2, 0, rect.right, rect.bottom ), &brush );
		dc.FillRect( CRect( 0, bmpInfo.bmHeight + 2, bmpInfo.bmWidth + 2, rect.bottom ), &brush );

		dc.SelectObject( pOldFont );
		dc.SelectObject( pOldBrush );
		dc.SelectObject( pOldPen );
	}
}
