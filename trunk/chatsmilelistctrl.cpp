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

CChatSmileListCtrl::CChatSmileListCtrl(): CListBox()
{
}

CChatSmileListCtrl::~CChatSmileListCtrl()
{
}

BOOL CChatSmileListCtrl::PreCreateWindow( CREATESTRUCT& cs )
{
	if ( !CListBox::PreCreateWindow(cs) ) return FALSE;
	cs.style |= WS_VISIBLE | WS_CHILD | LBS_HASSTRINGS | WS_HSCROLL | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT /*LBS_MULTICOLUMN*/;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
//	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW) /*, reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 ), NULL*/ );
	return TRUE;
}

int CChatSmileListCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( CListBox::OnCreate( lpCreateStruct ) == -1 ) return -1;

	AddString( "q1" );
	AddString( "q2" );
	AddString( "q3" );
	AddString( "q4" );

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
	CPoint imagePoint( focusRect.left, focusRect.top );

	if ( (lpDrawItemStruct->itemState & ODS_SELECTED) && (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)) ) {
		CBrush br( ::GetSysColor(COLOR_HIGHLIGHT) );
		pDC->FillRect( &focusRect, &br );
	} else if ( !(lpDrawItemStruct->itemState & ODS_SELECTED) && (lpDrawItemStruct->itemAction & ODA_SELECT) ) {
		CBrush br( ::GetSysColor(COLOR_WINDOW) );
		pDC->FillRect(&focusRect, &br);
	}

	if ( (lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS) ) {
		pDC->DrawFocusRect( &focusRect );
	} else if ( (lpDrawItemStruct->itemAction & ODA_FOCUS) && !(lpDrawItemStruct->itemState & ODS_FOCUS) ) {
		pDC->DrawFocusRect( &focusRect );
	}

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
	lpMeasureItemStruct->itemHeight = 40;
}
