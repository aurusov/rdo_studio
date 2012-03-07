/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      doc_serie.cpp
  \author    Захаров Павел
  \date      31.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/chart/doc_serie.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioDocSerie
// --------------------------------------------------------------------------------
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

void RDOStudioDocSerie::getLegendExtent( HDC &dc, CRect& rect, SIZE& size ) const
{
	size.cx = 0;
	size.cy = 0;
	if ( !showInLegend )
		return;
	
	CRect tmprect;
	tmprect.left = rect.left + 10 + marker_size * 2 + 5;
	tmprect.right = rect.right;
	tmprect.top = rect.top;
	tmprect.bottom = rect.bottom;
	::DrawText( dc, docSerieTitle.c_str(), docSerieTitle.length(), tmprect, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS | DT_CALCRECT );
	size.cy = tmprect.Height();
	if ( size.cy < marker_size * 2 ){
		size.cy = marker_size * 2;
	}
	size.cx = tmprect.right - rect.left;
	size.cy += 2;
}

void RDOStudioDocSerie::drawInLegend( HDC &dc, CRect &rect, const COLORREF text_color, SIZE& size ) const
{
	getLegendExtent( dc, rect, size );
	if ( !showInLegend )
		return;
	
	::SetTextColor( dc, text_color );

	HPEN pen = NULL;
	HPEN old_pen = NULL;
	
	HBRUSH brush_marker = NULL;
	HBRUSH old_brush = NULL;
	LOGBRUSH log_brush;
	log_brush.lbStyle = transparentMarker ? BS_HOLLOW : BS_SOLID;
	log_brush.lbColor = color;
	try {
		pen = ::CreatePen( PS_SOLID, 0, color );
		old_pen = (HPEN)::SelectObject( dc, pen );
		
		brush_marker = ::CreateBrushIndirect( &log_brush );
		old_brush = (HBRUSH)::SelectObject( dc, brush_marker );
		
		int middle = rect.top + ( size.cy - 2 ) / 2;
		if ( needDrawMarker )
			serie->drawMarker( dc, rect.left + 5 + marker_size, middle, marker, marker_size );
		
		::MoveToEx( dc, rect.left, middle, (LPPOINT)NULL );
		::LineTo( dc, rect.left + 10 + marker_size * 2, middle );
		
		CRect tmprect;
		tmprect.CopyRect( rect );
		tmprect.left += 10 + marker_size * 2 + 5;
		
		::DrawText( dc, docSerieTitle.c_str(), docSerieTitle.length(), tmprect, DT_LEFT | DT_SINGLELINE | DT_END_ELLIPSIS );

		::SelectObject( dc, old_pen );
		::DeleteObject( pen );
		pen = NULL;

		::SelectObject( dc, old_brush );
		::DeleteObject( brush_marker );
		brush_marker = NULL;
	} catch( ... ) {
		if ( pen ) {
			::SelectObject( dc, old_pen );
			::DeleteObject( pen );
			pen = NULL;
		}
		if ( brush_marker ) {
			::SelectObject( dc, old_brush );
			::DeleteObject( brush_marker );
			brush_marker = NULL;
		}
	}
}