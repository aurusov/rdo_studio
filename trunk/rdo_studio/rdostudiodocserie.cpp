#include "stdafx.h"
#include "rdostudiodocserie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioDocSerie
// ----------------------------------------------------------------------------
RDOStudioDocSerie::RDOStudioDocSerie( RDOTracerSerie* _serie ) :
	serie( _serie ),
	color( RGB( 0x00, 0x00, 0x00 ) ),
	marker( RDOSM_NONE ),
	marker_size( 3 ),
	showInLegend( true ),
	needDrawMarker( true ),
	transparentMarker( true )
{
	if( serie ) {
		docSerieTitle = serie->getTitle();
	}
}

RDOStudioDocSerie::~RDOStudioDocSerie()
{
}

CSize RDOStudioDocSerie::getLegendExtent( CDC &dc, CFont& font, CRect& rect ) const
{
	CSize size( 0 , 0 );
	if ( !showInLegend )
		return size;
	CFont*  oldFont = dc.SelectObject( &font );
	CRect tmprect;
	tmprect.left = rect.left + 10 + marker_size * 2 + 5;
	tmprect.right = rect.right;
	tmprect.top = rect.top;
	tmprect.bottom = rect.bottom;
	dc.DrawText( docSerieTitle.c_str(), &tmprect, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS | DT_CALCRECT );
	size.cy = tmprect.Height();
	if ( size.cy < marker_size * 2 ){
		size.cy = marker_size * 2;
	}
	size.cx = tmprect.right - rect.left;
	dc.SelectObject( oldFont );
	size.cy += 2;
	return size;
}

CSize RDOStudioDocSerie::drawInLegend( CDC &dc, CRect &rect, CFont& font, const COLORREF text_color ) const
{
	CSize size = getLegendExtent( dc, font, rect );
	if ( !showInLegend )
		return size;
	int oldBkMode = dc.SetBkMode( TRANSPARENT );
	CPen pen;
	pen.CreatePen( PS_SOLID, 0, color );
	CPen* pOldPen = dc.SelectObject( &pen );

	CFont*  oldFont = dc.SelectObject( &font );
	COLORREF old_color = dc.SetTextColor( text_color );

	int middle = rect.top + ( size.cy - 2 ) / 2;
	if ( needDrawMarker )
		serie->drawMarker( dc, rect.left + 5 + marker_size, middle, color, marker, marker_size, transparentMarker );
	dc.MoveTo( rect.left, middle );
	dc.LineTo( rect.left + 10 + marker_size * 2, middle );
	CRect tmprect;
	tmprect.CopyRect( rect );
	tmprect.left += 10 + marker_size * 2 + 5;
	dc.DrawText( docSerieTitle.c_str(), &tmprect, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS );

	dc.SetTextColor( old_color );
	dc.SelectObject( oldFont );
	dc.SelectObject( pOldPen );
	dc.SetBkMode( oldBkMode );

	return size;
}