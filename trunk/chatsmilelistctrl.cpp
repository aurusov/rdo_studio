#include "stdafx.h"
#include "chatsmilelistctrl.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatSmileListCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatSmileListCtrl, CListBox )
	//{{AFX_MSG_MAP(CChatSmileListCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()

CChatSmileListCtrl::CChatSmileListCtrl():
	CListBox(),
	smile_max_width( 0 ),
	text_focus_height( 0 )
{
}

CChatSmileListCtrl::~CChatSmileListCtrl()
{
}

BOOL CChatSmileListCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CListBox::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT, LBS_MULTICOLUMN;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
//	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) /*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );
	return TRUE;
}

int CChatSmileListCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CListBox::OnCreate( lpCreateStruct ) == -1 ) return -1;

	HFONT hf = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	if ( hf ) {
		SetFont( CFont::FromHandle( hf ) );
	}

	CDC* dc = GetDC();
	text_focus_height = dc->DrawText( ":)", CRect( 0, 0, 1, 1 ), DT_SINGLELINE | DT_CALCRECT );
	if ( text_focus_height <= 0 ) {
		text_focus_height = 10;
	}
	text_focus_height += 2;
	ReleaseDC( dc );
	for ( int i = CChatSmile::smile; i <= CChatSmile::tomato; i++ ) {
		CChatSmile* smile = list.addSmile( static_cast<CChatSmile::Type>(i), this );
		smile->ShowWindow( SW_HIDE );
		int index = AddString( CChatSmileList::getStr( static_cast<CChatSmile::Type>(i) ).c_str() );
		SetItemData( index, reinterpret_cast<DWORD>(smile) );
		CSize size = smile->GetSize();
		int smile_height = size.cy + 2;
		SetItemHeight( index, text_focus_height > smile_height ? text_focus_height : smile_height );
		if ( size.cx > smile_max_width ) {
			smile_max_width = size.cx;
		}
	}
	list.setBgColor( ::GetSysColor( COLOR_WINDOW ) );

	return 0;
}

void CChatSmileListCtrl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDC* pDC = CDC::FromHandle( lpDrawItemStruct->hDC );

	if ( static_cast<int>(lpDrawItemStruct->itemID) < 0 ) {
		return;
		if ( (lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS) ) {
			pDC->DrawFocusRect( &lpDrawItemStruct->rcItem );
		} else if ( (lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS) ) {
			pDC->DrawFocusRect( &lpDrawItemStruct->rcItem ); 
		}
		return;
	}

	CRect  focusRect( lpDrawItemStruct->rcItem );
	CRect  textRect( focusRect );
	CPoint smilePoint( focusRect.left, focusRect.top );

	CChatSmile* smile = reinterpret_cast<CChatSmile*>(GetItemData( lpDrawItemStruct->itemID ));
	CSize smile_size = smile->GetSize();
	smilePoint.x  += 1 + ( smile_max_width - smile_size.cx ) / 2;
	smilePoint.y  += 1;

	textRect.left += smile_max_width + 1;

//	focusRect.left = textRect.left - 1;
//	focusRect.DeflateRect( 0, (focusRect.Height() - text_focus_height) / 2 );

	if ( (lpDrawItemStruct->itemState & ODS_SELECTED) && (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)) ) {
		COLORREF color = ::GetSysColor( COLOR_HIGHLIGHT );
		pDC->FillSolidRect( &focusRect, color );
		smile->setBgColor( color );
	} else if ( !(lpDrawItemStruct->itemState & ODS_SELECTED) && (lpDrawItemStruct->itemAction & ODA_SELECT) ) {
		COLORREF color = ::GetSysColor( COLOR_WINDOW );
		pDC->FillSolidRect( &focusRect, color );
		smile->setBgColor( color );
	}

	if ( (lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS) ) {
		pDC->DrawFocusRect( &focusRect );
	} else if ( (lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS) ) {
		pDC->DrawFocusRect( &focusRect );
	}

	smile->MoveWindow( smilePoint.x, smilePoint.y, smile_size.cx, smile_size.cy );
	smile->ShowWindow( SW_SHOW );

	int prev_bkMode = pDC->SetBkMode( TRANSPARENT );
	COLORREF prev_textColor;

	if ( lpDrawItemStruct->itemState & ODS_SELECTED ) {
		prev_textColor = pDC->SetTextColor( ::GetSysColor(COLOR_HIGHLIGHTTEXT) );
	} else if ( lpDrawItemStruct->itemState & ODS_DISABLED ) {
		prev_textColor = pDC->SetTextColor( ::GetSysColor(COLOR_GRAYTEXT) );
	} else {
		prev_textColor = pDC->SetTextColor( ::GetSysColor(COLOR_WINDOWTEXT) );
	}

	CString str;
	GetText( lpDrawItemStruct->itemID, str );

	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if ( GetStyle() & LBS_USETABSTOPS ) {
		nFormat |= DT_EXPANDTABS;
	}
	pDC->DrawText( str, -1, &textRect, nFormat );

	pDC->SetTextColor( prev_textColor );
	pDC->SetBkMode( prev_bkMode );
}

void CChatSmileListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
}
