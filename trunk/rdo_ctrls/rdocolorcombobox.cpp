#include "rdo_ctrls_pch.h"
#include "rdocolorcombobox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ----------------------------------------------------------------------------
// ---------- RDOColorComboBox
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( RDOColorComboBox, CComboBox )
	//{{AFX_MSG_MAP(RDOColorComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

RDOColorComboBox::RDOColorComboBox(): CComboBox()
{
}

RDOColorComboBox::~RDOColorComboBox()
{
}

void RDOColorComboBox::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDC dc;

	if ( !dc.Attach( lpDrawItemStruct->hDC ) ) return;

	CRect rectItem( lpDrawItemStruct->rcItem );

	int itemState = lpDrawItemStruct->itemState;

	COLORREF colorText = GetSysColor( COLOR_WINDOWTEXT );
	COLORREF colorBg;
	if ( itemState & ODS_SELECTED ) {
		colorBg = GetSysColor( COLOR_HIGHLIGHT );
		dc.SetTextColor( ( 0x00FFFFFF & ~(colorText) ) );
	} else {
		colorBg = GetSysColor( COLOR_WINDOW );
		dc.SetTextColor( colorText );
	}
	dc.SetBkColor( colorBg );
	if ( !(itemState & ODS_DISABLED) ) dc.FillSolidRect( &rectItem, colorBg );

	if ( itemState & ODS_FOCUS ) dc.DrawFocusRect( &rectItem );

	int itemIndex = lpDrawItemStruct->itemID;
	if ( itemIndex != -1 ) {

		CString str;
		GetLBText( itemIndex, str );

		COLORREF color = GetItemData( itemIndex );
//		if ( itemState & ODS_DISABLED ) {
//			color = GetSysColor( COLOR_INACTIVECAPTIONTEXT );
//		} else {
//			color = GetItemData( itemIndex );
//		}

		CRect rectColor( rectItem );
		CRect rectText( rectItem );

		int rectColorRight = 25;

		CSize sz = dc.GetTextExtent( str );

		rectText.left += rectColorRight + 2;
		rectText.top  += (rectText.Height() - sz.cy) / 2;

		rectColor.DeflateRect( 2, 2 );
		rectColor.right = rectColorRight;

		dc.FillSolidRect( &rectColor, color );

		dc.SetBkMode( TRANSPARENT );
		dc.TextOut( rectText.left, rectText.top, str );

		CBrush frameBrush;
		frameBrush.CreateStockObject( BLACK_BRUSH );
		dc.FrameRect( &rectColor, &frameBrush );
	}

	dc.Detach();
}

void RDOColorComboBox::MeasureItem( LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/ )
{
}

void RDOColorComboBox::setItemHeight( const int height )
{
	SetItemHeight( -1, height );
	SetItemHeight(  0, height );
}

void RDOColorComboBox::insertBaseColors()
{
	insertColor( RGB( 0x00, 0x80, 0x00 ) );
	insertColor( RGB( 0x00, 0x00, 0x80 ) );
	insertColor( RGB( 0x80, 0x80, 0x80 ) );
	insertColor( RGB( 0x80, 0x00, 0x80 ) );
	insertColor( RGB( 0xFF, 0x00, 0x00 ) );
	insertColor( RGB( 0x00, 0xFF, 0x00 ) );
	insertColor( RGB( 0x00, 0x00, 0x00 ) );
	insertColor( RGB( 0xFF, 0xFF, 0xFF ) );
	insertColor( RGB( 0x80, 0x80, 0x00 ) );
	insertColor( RGB( 0xC0, 0xC0, 0xC0 ) );
	insertColor( RGB( 0x80, 0x00, 0x00 ) );
	insertColor( RGB( 0x00, 0x80, 0x80 ) );
	insertColor( RGB( 0xFF, 0xFF, 0x00 ) );
	insertColor( RGB( 0x00, 0x00, 0xFF ) );
	insertColor( RGB( 0xFF, 0x00, 0xFF ) );
	insertColor( RGB( 0x00, 0xFF, 0xFF ) );
}

int RDOColorComboBox::insertColor( const COLORREF color, CString colorName )
{
	if ( colorName.IsEmpty() ) {
		switch ( color ) {
			case RGB( 0x00, 0x80, 0x00 ): colorName = "Green"; break;
			case RGB( 0x00, 0x00, 0x80 ): colorName = "Navy"; break;
			case RGB( 0x80, 0x80, 0x80 ): colorName = "Grey"; break;
			case RGB( 0x80, 0x00, 0x80 ): colorName = "Purple"; break;
			case RGB( 0xFF, 0x00, 0x00 ): colorName = "Red"; break;
			case RGB( 0x00, 0xFF, 0x00 ): colorName = "Lime"; break;
			case RGB( 0x00, 0x00, 0x00 ): colorName = "Black"; break;
			case RGB( 0xFF, 0xFF, 0xFF ): colorName = "White"; break;
			case RGB( 0x80, 0x80, 0x00 ): colorName = "Olive"; break;
			case RGB( 0xC0, 0xC0, 0xC0 ): colorName = "Silver"; break;
			case RGB( 0x80, 0x00, 0x00 ): colorName = "Maroon"; break;
			case RGB( 0x00, 0x80, 0x80 ): colorName = "Teal"; break;
			case RGB( 0xFF, 0xFF, 0x00 ): colorName = "Yellow"; break;
			case RGB( 0x00, 0x00, 0xFF ): colorName = "Blue"; break;
			case RGB( 0xFF, 0x00, 0xFF ): colorName = "Fushcia"; break;
			case RGB( 0x00, 0xFF, 0xFF ): colorName = "Aqua"; break;
			default: colorName.Format( "[%d, %d, %d]", GetRValue(color), GetGValue(color), GetBValue(color) );
		}
	}
	int itemIndex = findItem( color );
	if ( itemIndex >= 0 ) {
		DeleteString( itemIndex );
	}
	int res = InsertString( 0, colorName );
	if ( res >= 0 ) {
		SetItemData( res, color );
		SetCurSel( res );
	}
	return res;
}

int RDOColorComboBox::findItem( const CString colorName )
{
	return FindStringExact( -1, colorName );
}

int RDOColorComboBox::findItem( const COLORREF color )
{
	for ( int i = 0; i < GetCount(); i++ ) {
		if ( GetItemData( i ) == color ) return i;
	}
	return CB_ERR;
}

CString RDOColorComboBox::getCurrentColorName() const
{
	CString s = "";
	int itemIndex = GetCurSel();
	if ( itemIndex >= 0 ) {
		GetLBText( itemIndex, s );
	}
	return s;
}

COLORREF RDOColorComboBox::getCurrentColor() const
{
	int itemIndex = GetCurSel();
	if ( itemIndex >= 0 ) {
		return GetItemData( itemIndex );
	} else {
		return RGB( 0xFF, 0xFF, 0xFF );
	}
}

void RDOColorComboBox::setCurrentColor( const CString& colorName )
{
	int itemIndex = findItem( colorName );
	if ( itemIndex >= 0 ) {
		SetCurSel( itemIndex );
	} else {
		SetCurSel( -1 );
	}
}

void RDOColorComboBox::setCurrentColor( const COLORREF color )
{
	int itemIndex = findItem( color );
	if ( itemIndex >= 0 ) {
		SetCurSel( itemIndex );
	} else {
		SetCurSel( -1 );
	}
}
