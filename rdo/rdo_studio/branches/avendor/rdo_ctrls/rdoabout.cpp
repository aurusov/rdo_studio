#include "rdo_ctrls_pch.h"
#include "rdoabout.h"

#include <shellapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOAbout
// ----------------------------------------------------------------------------
#define ID_EMAIL   2000
#define ID_WEBSITE 2001

const int pixmapWidth       = ::GetSystemMetrics( SM_CYICON );
const int pixmapBorder      = 8;
const int pixmapRight       = pixmapWidth + pixmapBorder * 2;
const int fontCaptionTop    = 20;
const int fontCaptionBorder = 7;
const int infoBorder        = 15;
const int emailBorder       = 5;
const int emailBottomBorder = 2;
const int webSiteBorder     = 5;
const int buttonOKBorder    = 10;

BEGIN_MESSAGE_MAP( RDOAbout, CDialog )
	//{{AFX_MSG_MAP(RDOAbout)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_NOTIFY( NM_RDOLINK_CLICK, ID_EMAIL  , OnEMailLinkLButtonDown )
	ON_NOTIFY( NM_RDOLINK_CLICK, ID_WEBSITE, OnWebSiteLinkLButtonDown )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOAbout::RDOAbout() :
	CDialog(),
	dx( 0 ),
	dy( 0 ),
	isMouseDown( false ),
	width( 280 ),
	caption( "" ),
	email( "e-mail :" ),
	webSite( "web site :" ),
	captionBold( true ),
	BMSTUBold( false ),
	telBold( false ),
	emailBold( false ),
	webSiteBold( false ),
	emailLinkText( "evv@rk9.bmstu.ru" ),
	webSiteLinkText( "www.rk9.bmstu.ru" ),
	buttonOkText( "Close" ),
	emailLinkBold( false ),
	webSiteLinkBold( false ),
	buttonOkBold( true ),
	hPixmap( NULL ),
	bgPixmap( RGB(0xD0, 0xF0, 0xEF) ),
	fgCaption( RGB(0x00, 0x40, 0x80) ),
	bgCaption( RGB(0xA0, 0xF0, 0xEF) ),
	fgInfo( RGB(0x00, 0x00, 0x00) ),
	bgInfo( RGB(0xEF, 0xF8, 0xF0) ),
	fgLink( RGB(0x00, 0x40, 0x80) ),
	fgButtonOK( RGB(0x00, 0x40, 0x80) ),
	bgButtonOK( RGB(0xD0, 0xF0, 0xEF) )
{
	if ( PRIMARYLANGID(GetSystemDefaultLangID()) == LANG_RUSSIAN ) {
		BMSTU = "Московский Государственный Технический Университет им. Н.Э.Баумана, кафедра \"Компьютерные системы автоматизации производства\" (РК-9), Москва, Россия";
		tel   = "Тел. (095) 263-6388, (095) 263-6954, (095) 263-6049\r\nФакс (095) 263-6170";
	} else {
		BMSTU = "Moscow State Bauman University of Technology, \"Computer-aided systems for manufacturing automatization\" Department (RK-9), Moscow, Russia";
		tel   = "Tel (095) 263-6388, (095) 263-6954, (095) 263-6049\r\nFax (095) 263-6170";
	}
	TCHAR szExeName[ MAX_PATH + 1 ];
	if ( ::GetModuleFileName( NULL, szExeName, MAX_PATH ) ) {
		DWORD dwHnd;
		DWORD size = ::GetFileVersionInfoSize( szExeName, &dwHnd );
		if ( size ) {
			void* pBuffer = malloc( size );
			if ( pBuffer != NULL ) {
				if ( ::GetFileVersionInfo( szExeName, dwHnd, size, pBuffer ) ) {
					DWORD* pTranslation;
					UINT   length;
					if ( ::VerQueryValue( pBuffer, _T("\\VarFileInfo\\Translation"), (void**)&pTranslation, &length ) ) {
						DWORD translation = *pTranslation;
						char key[2000];
						wsprintf( key, _T("\\StringFileInfo\\%04x%04x\\ProductName"), LOWORD( translation ), HIWORD( translation ) );
						char* productName;
						if ( ::VerQueryValue( pBuffer, key, (void**)&productName, &length ) ) {
							VS_FIXEDFILEINFO* fixedInfo;
							if ( ::VerQueryValue( pBuffer, _T("\\"), (void**)&fixedInfo, &length ) ) {
								caption.Format( "%s     version %u.%u (build %u)", productName, HIWORD( fixedInfo->dwProductVersionMS ), LOWORD( fixedInfo->dwProductVersionMS ), LOWORD( fixedInfo->dwProductVersionLS ) );
							}
						}
					}
				}
				free( pBuffer );
			}
		}
	}
	hPixmap = AfxGetMainWnd()->GetIcon( TRUE );
}

RDOAbout::~RDOAbout()
{
}

BOOL RDOAbout::OnInitDialog()
{
	CDialog::OnInitDialog();

	CDC* dc = GetDC();
	// prepare fonts for caption and info
	HFONT hf = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	if ( hf ) {
		CFont* f = CFont::FromHandle( hf );
		if ( f ) {
			LOGFONT lf;
			f->GetLogFont( &lf );

			lf.lfWeight = BMSTUBold ? FW_BOLD : FW_NORMAL;
			fontBMSTU.CreateFontIndirect( &lf );

			lf.lfWeight = telBold ? FW_BOLD : FW_NORMAL;
			fontTel.CreateFontIndirect( &lf );

			lf.lfWeight = emailBold ? FW_BOLD : FW_NORMAL;
			fontEmail.CreateFontIndirect( &lf );

			lf.lfWeight = webSiteBold ? FW_BOLD : FW_NORMAL;
			fontWebSite.CreateFontIndirect( &lf );

			int dcaps = GetDeviceCaps( dc->m_hDC, LOGPIXELSY );
			int size = -lf.lfHeight * 72 / dcaps;
			lf.lfHeight = -MulDiv( size + 1, dcaps, 72 );
			lf.lfWeight = captionBold ? FW_BOLD : FW_NORMAL;
			fontCaption.CreateFontIndirect( &lf );
		}
	}

	CRect r;
	GetClientRect( r );

	// calc caption rect
	CFont* prevFont = dc->SelectObject( &fontCaption );
	rCaption.left   = r.left;
	rCaption.top    = fontCaptionTop;
	rCaption.right  = r.right;
	rCaption.bottom = 1;
	int h = dc->DrawText( caption, rCaption, DT_SINGLELINE | DT_CENTER | DT_TOP | DT_CALCRECT );
	rCaption.left   = r.left;
	rCaption.right  = r.right;
	rCaption.bottom = rCaption.top + h + fontCaptionBorder * 2;

	// calc BMSTU rect
	dc->SelectObject( &fontBMSTU );
	rBMSTU.left   = r.left + pixmapRight + infoBorder;
	rBMSTU.top    = rCaption.bottom + infoBorder;
	rBMSTU.right  = r.right - infoBorder;
	rBMSTU.bottom = 1;
	h = dc->DrawText( BMSTU, rBMSTU, DT_WORDBREAK | DT_CENTER | DT_TOP | DT_CALCRECT );
	rBMSTU.left   = r.left + pixmapRight + infoBorder;
	rBMSTU.right  = r.right - infoBorder;
	rBMSTU.bottom = rBMSTU.top + h;

	// calc tel rect
	dc->SelectObject( &fontTel );
	rTel.left   = rBMSTU.left;
	rTel.top    = rBMSTU.bottom + infoBorder;
	rTel.right  = rBMSTU.right;
	rTel.bottom = 1;
	h = dc->DrawText( tel, rTel, DT_WORDBREAK | DT_RIGHT | DT_TOP | DT_CALCRECT );
	rTel.left   = rBMSTU.left;
	rTel.right  = rBMSTU.right;
	rTel.bottom = rTel.top + h;

	int nextBottom = rTel.bottom + infoBorder;
	bool showEmail = false;

	// calc email rect
	if ( !emailLinkText.IsEmpty() ) {
		showEmail = true;

		emailLink.Create( emailLinkText, CRect( 0, 0, 1, 1 ), this, ID_EMAIL );
		emailLink.setTextColor( fgLink );
		emailLink.setBgColor( bgInfo );
		emailLink.setFontBold( emailLinkBold );
		emailLink.autoSize();
		emailLink.GetWindowRect( rEmailLink );
		ScreenToClient( rEmailLink );
		rEmailLink.left   = rBMSTU.right - ( rEmailLink.right - rEmailLink.left );
		rEmailLink.right  = rBMSTU.right;
		rEmailLink.bottom = ( rEmailLink.bottom - rEmailLink.top ) + nextBottom;
		rEmailLink.top    = nextBottom;
		emailLink.MoveWindow( rEmailLink );

		dc->SelectObject( &fontEmail );
		rEmail.left   = rBMSTU.left;
		rEmail.top    = nextBottom;
		rEmail.right  = rEmailLink.left - emailBorder;
		rEmail.bottom = 1;
		h = dc->DrawText( email, rEmail, DT_WORDBREAK | DT_RIGHT | DT_TOP | DT_CALCRECT );
		rEmail.left   = rBMSTU.left;
		rEmail.right  = rEmailLink.left - emailBorder;
		rEmail.bottom = rEmail.top + h;

		nextBottom = rEmail.bottom + emailBottomBorder;
	}

	// calc webSite rect
	if ( !webSiteLinkText.IsEmpty() ) {
		webSiteLink.Create( webSiteLinkText, CRect( 0, 0, 1, 1 ), this, ID_WEBSITE );
		webSiteLink.setTextColor( fgLink );
		webSiteLink.setBgColor( bgInfo );
		webSiteLink.setFontBold( webSiteLinkBold );
		webSiteLink.autoSize();
		webSiteLink.GetWindowRect( rWebSiteLink );
		ScreenToClient( rWebSiteLink );
		rWebSiteLink.left   = rBMSTU.right - ( rWebSiteLink.right - rWebSiteLink.left );
		rWebSiteLink.right  = rBMSTU.right;
		rWebSiteLink.bottom = ( rWebSiteLink.bottom - rWebSiteLink.top ) + nextBottom;
		rWebSiteLink.top    = nextBottom;
		webSiteLink.MoveWindow( rWebSiteLink );

		dc->SelectObject( &fontWebSite );
		rWebSite.left   = rBMSTU.left;
		rWebSite.top    = nextBottom;
		rWebSite.right  = rWebSiteLink.left - webSiteBorder;
		rWebSite.bottom = 1;
		h = dc->DrawText( webSite, rWebSite, DT_WORDBREAK | DT_RIGHT | DT_TOP | DT_CALCRECT );
		rWebSite.left   = rBMSTU.left;
		rWebSite.right  = rWebSiteLink.left - webSiteBorder;
		rWebSite.bottom = rWebSite.top + h;

		if ( showEmail ) {
			if ( rEmail.right < rWebSite.right ) {
				rWebSite.right = rEmail.right;
			} else {
				rEmail.right = rWebSite.right;
			}
		}

		nextBottom = rWebSite.bottom + infoBorder;

	} else {

		nextBottom = rEmail.bottom + infoBorder;

	}

	// calc bottom rect
	rBottom.left   = rCaption.left;
	rBottom.top    = nextBottom;
	rBottom.right  = rCaption.right;
	rBottom.bottom = rBottom.top + rCaption.bottom - rCaption.top;

	// calc button OK
	buttonOK.Create( buttonOkText, CRect( 0, 0, 1, 1 ), this, IDOK );
	buttonOK.setTextColor( fgButtonOK );
	buttonOK.setBgColor( bgButtonOK );
	buttonOK.setFontBold( buttonOkBold );
	buttonOK.autoSize();
	buttonOK.GetWindowRect( rButtonOK );
	ScreenToClient( rButtonOK );
	rButtonOK.InflateRect( 14, 4 );
	int wButtonOK = rButtonOK.right - rButtonOK.left;
	int hButtonOK = rButtonOK.bottom - rButtonOK.top;
	rButtonOK.left   = r.right - wButtonOK - buttonOKBorder;
	rButtonOK.right  = rButtonOK.left + wButtonOK;
	rButtonOK.top    = rBottom.bottom + buttonOKBorder;
	rButtonOK.bottom = rButtonOK.top + hButtonOK;
	CRect rButtonOKLink( rButtonOK );
	rButtonOKLink.DeflateRect( 14, 4 );
	buttonOK.MoveWindow( rButtonOKLink );

	// calc window rect
	CRect rWindow;
	GetWindowRect( rWindow );
	int wWindow = rButtonOK.right + buttonOKBorder;
	int hWindow = rButtonOK.bottom + buttonOKBorder;
	rWindow.left = (::GetSystemMetrics( SM_CXSCREEN ) - wWindow ) / 2;
	rWindow.top  = (::GetSystemMetrics( SM_CYSCREEN ) - hWindow ) / 2;
	rWindow.right  = rWindow.left + wWindow;
	rWindow.bottom = rWindow.top + hWindow;
	MoveWindow( &rWindow );

	dc->SelectObject( prevFont );

	return true;
}

void RDOAbout::OnLButtonDown( UINT nFlags, CPoint point )
{
	CDialog::OnLButtonDown( nFlags, point );
	dx = -point.x;
	dy = -point.y;
	isMouseDown = true;
	SetCapture();
}

void RDOAbout::OnLButtonUp( UINT nFlags, CPoint point )
{
	CDialog::OnLButtonUp( nFlags, point );
	isMouseDown = false;
	::ReleaseCapture();
}

void RDOAbout::OnMouseMove( UINT nFlags, CPoint point )
{
	CDialog::OnMouseMove( nFlags, point );
	if ( isMouseDown ) {
		CRect r;
		GetWindowRect( r );
		MoveWindow( r.left + dx + point.x, r.top + dy + point.y, r.right - r.left, r.bottom - r.top, true );
	}
}

void RDOAbout::OnPaint()
{
	CPaintDC dc( this );
	CRect r;
	GetClientRect( r );

	// fill all client's client
	dc.FillSolidRect( r.left, r.top, pixmapRight, r.bottom, bgPixmap );
	dc.FillSolidRect( r.left + pixmapRight, r.top, r.right - pixmapRight, r.bottom, bgInfo );

	// caption
	CPen penCaption( PS_SOLID, 2, bgInfo );
	CBrush brushCaption( bgCaption );
	CPen*   prevPen   = dc.SelectObject( &penCaption );
	CBrush* prevBrush = dc.SelectObject( &brushCaption );
	CFont*  prevFont  = dc.SelectObject( &fontCaption );
	dc.Rectangle( rCaption );
	dc.SetBkMode( TRANSPARENT );
	dc.SetTextColor( fgCaption );
	dc.DrawText( caption, rCaption, DT_SINGLELINE | DT_CENTER | DT_VCENTER );

	// draw pixmap
	if ( hPixmap ) {
		dc.DrawIcon( pixmapBorder, rCaption.bottom + pixmapBorder, hPixmap );
	}

	// BMSTU and tel
	dc.SelectObject( &fontBMSTU );
	dc.SetBkMode( TRANSPARENT );
	dc.SetTextColor( fgInfo );
	dc.DrawText( BMSTU, rBMSTU, DT_WORDBREAK | DT_CENTER | DT_TOP );
	dc.SelectObject( &fontTel );
	dc.DrawText( tel, rTel, DT_WORDBREAK | DT_RIGHT | DT_TOP );

	// email
	if ( !emailLinkText.IsEmpty() ) {
		dc.SelectObject( &fontEmail );
		dc.SetBkMode( TRANSPARENT );
		dc.SetTextColor( fgInfo );
		dc.DrawText( email, rEmail, DT_WORDBREAK | DT_RIGHT | DT_TOP );
	}

	// webSite
	if ( !webSiteLinkText.IsEmpty() ) {
		dc.SelectObject( &fontWebSite );
		dc.SetBkMode( TRANSPARENT );
		dc.SetTextColor( fgInfo );
		dc.DrawText( webSite, rWebSite, DT_WORDBREAK | DT_RIGHT | DT_TOP );
	}

	// bottom
	dc.SelectObject( &penCaption );
	dc.SelectObject( &brushCaption );
	dc.Rectangle( rBottom );

	// button OK bg
	dc.FillSolidRect( rButtonOK, bgButtonOK );

	// draw client border
	CPen penBorder( PS_SOLID, 1, bgCaption );
	LOGBRUSH lb;
	lb.lbStyle = BS_NULL;
	CBrush brushBorder;
	brushBorder.CreateBrushIndirect( &lb );
	dc.SelectObject( &penBorder );
	dc.SelectObject( &brushBorder );
	dc.Rectangle( r.left, r.top, r.right, r.bottom );

	dc.SelectObject( prevPen );
	dc.SelectObject( prevBrush );
	dc.SelectObject( prevFont );
}

void RDOAbout::OnEMailLinkLButtonDown( NMHDR* pNotifyStruct, LRESULT* result )
{
	::ShellExecute( m_hWnd, "open", "mailto:" + emailLinkText, 0, 0, SW_SHOWNORMAL );
	*result = 0;
}

void RDOAbout::OnWebSiteLinkLButtonDown( NMHDR* pNotifyStruct, LRESULT* result )
{
	::ShellExecute( m_hWnd, "open", webSiteLinkText, 0, 0, SW_SHOWNORMAL );
	*result = 0;
}

int RDOAbout::DoModal()
{
	int res = -1;

	int nBufferSize = sizeof(DLGTEMPLATE) + 3 * sizeof(WORD);
	nBufferSize = (nBufferSize + 3) & ~3;

	HLOCAL hLocal = LocalAlloc( LHND, nBufferSize );
	if ( !hLocal ) return res;

	BYTE *pBuffer = (BYTE*)LocalLock( hLocal );
	if ( !pBuffer ) {
		LocalFree( hLocal );
		return res;
	}
	BYTE *pDest = pBuffer;

	DLGTEMPLATE m_dlgMemory;
	m_dlgMemory.style = WS_VISIBLE | WS_POPUP;
	m_dlgMemory.dwExtendedStyle = 0;
	m_dlgMemory.cdit = 0;
	m_dlgMemory.x = 0;
	m_dlgMemory.y = 0;
	m_dlgMemory.cx = width;
	m_dlgMemory.cy = 1;
	memcpy( pDest, &m_dlgMemory, sizeof(DLGTEMPLATE) );
	pDest += sizeof(DLGTEMPLATE);
	*(WORD*)pDest = 0;
	pDest += sizeof(WORD);
	*(WORD*)pDest = 0;
	pDest += sizeof(WORD);
	*(WORD*)pDest = 0;
	pDest += sizeof(WORD);

	ASSERT( pDest - pBuffer == nBufferSize );

	InitModalIndirect( (DLGTEMPLATE*)pBuffer );

	res = CDialog::DoModal();

	LocalUnlock( hLocal );
	LocalFree( hLocal );

	return res;
}

int RDOAbout::getCheckedColor( const int color )
{
	if ( color < 0 ) return 0;
	if ( color > 255 ) return 255;
	return color;
}

void RDOAbout::changeColor( const int r, const int g, const int b )
{
	bgPixmap   = RGB( getCheckedColor(GetRValue(bgPixmap) + r), getCheckedColor(GetGValue(bgPixmap) + g), getCheckedColor(GetBValue(bgPixmap) + b) );
	fgCaption  = RGB( getCheckedColor(GetRValue(fgCaption) + r), getCheckedColor(GetGValue(fgCaption) + g), getCheckedColor(GetBValue(fgCaption) + b) );
	bgCaption  = RGB( getCheckedColor(GetRValue(bgCaption) + r), getCheckedColor(GetGValue(bgCaption) + g), getCheckedColor(GetBValue(bgCaption) + b) );
	fgInfo     = RGB( getCheckedColor(GetRValue(fgInfo) + r), getCheckedColor(GetGValue(fgInfo) + g), getCheckedColor(GetBValue(fgInfo) + b) );
	bgInfo     = RGB( getCheckedColor(GetRValue(bgInfo) + r), getCheckedColor(GetGValue(bgInfo) + g), getCheckedColor(GetBValue(bgInfo) + b) );
	fgLink     = RGB( getCheckedColor(GetRValue(fgLink) + r), getCheckedColor(GetGValue(fgLink) + g), getCheckedColor(GetBValue(fgLink) + b) );
	fgButtonOK = fgCaption;
	bgButtonOK = bgPixmap;
}
