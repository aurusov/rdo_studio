#include "stdafx.h"
#include "chatlistboxctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatListBoxCtrl
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatListBoxCtrl, CListBox )
	//{{AFX_MSG_MAP(CChatListBoxCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatListBoxCtrl::CChatListBoxCtrl():
	CListBox(),
	imageList( NULL )
{
}

CChatListBoxCtrl::~CChatListBoxCtrl()
{
}

bool CChatListBoxCtrl::isImage() const
{
	return imageList && imageList->GetImageCount();
}

CSize CChatListBoxCtrl::getImageSize() const
{
	if ( isImage() ) {
		IMAGEINFO imageInfo;
		if ( imageList->GetImageInfo( 0, &imageInfo ) ) {
			return CSize( imageInfo.rcImage.right - imageInfo.rcImage.left, imageInfo.rcImage.bottom - imageInfo.rcImage.top );
		}
	}
	return CSize( 0, 0 );
}

void CChatListBoxCtrl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
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

	if ( isImage() ) {
		imagePoint.x  += 1;
		imagePoint.y  += 1;
		textRect.left += getImageSize().cx + 3;
	} else {
		textRect.left += 2;
	}

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

	int image = (int)lpDrawItemStruct->itemData;
	if( imageList && ( image != -1 ) ) {
		imageList->Draw( pDC, image, imagePoint, ILD_NORMAL );
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

void CChatListBoxCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if ( isImage() ) {
		lpMeasureItemStruct->itemHeight = getImageSize().cy + 2;
	}
}

int CChatListBoxCtrl::addString( const std::string& str, const int image )
{
	int ret = CListBox::AddString( str.c_str() );
	if ( ret >= 0 ) {
		SetItemData( ret, image );
	}
	return ret;
}

int CChatListBoxCtrl::insertString( int index, const std::string& str, const int image )
{
	int ret = CListBox::InsertString( index, str.c_str() );
	if ( ret >= 0 ) {
		SetItemData( ret, image );
	}
	return ret;
}

void CChatListBoxCtrl::setItemImage( const int index, const int image )
{
	SetItemData( index, image );
	CRect r;
	if ( GetItemRect( index, &r ) != LB_ERR ) {
		RedrawWindow( r );
	}
}

void CChatListBoxCtrl::setImageList( CImageList* _imageList )
{
	imageList = _imageList;
}
