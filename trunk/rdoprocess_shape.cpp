#include "stdafx.h"
#include "rdoprocess_shape.h"
#include "rdoprocess_flowchart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPShape
// ----------------------------------------------------------------------------
RPShape::RPShape( RPObject* _parent, RPFlowChart* _flowchart, const rp::string& _name ):
	RPChartObject( _parent, _flowchart->flowobj, _flowchart, _name )
//	snap_to_point( 0, 0 )
{
	flowchart->insertShape( this );
}

RPShape::~RPShape()
{
	flowchart->deleteShape( this );
}

void RPShape::setPosition( int x, int y )
{
	RPChartObject::setPosition( x, y );
	flowchart->snapToGrid( this );
}
/*
void RPShape::meshToGlobal()
{
	if ( pa_global.size() != pa_src.size() ) {
		pa_global.resize( pa_src.size() );
	}
	trans tr( globalMatrix() );
	std::transform( pa_src.begin(), pa_src.end(), pa_global.begin(), tr );
}
*/
void RPShape::transformToGlobal()
{
	if ( pa_global.size() != pa_src.size() ) {
		pa_global.resize( pa_src.size() );
	}
	trans tr( globalMatrix() );
	std::transform( pa_src.begin(), pa_src.end(), pa_global.begin(), tr );
}

rp::rect RPShape::getBoundingRect( bool global ) const
{
	rp::rect bound_rect;
	if ( !pa_src.empty() ) {
		int x_min = pa_src[0].x;
		int y_min = pa_src[0].y;
		int x_max = pa_src[0].x;
		int y_max = pa_src[0].y;
		std::vector< CPoint >::const_iterator it = pa_src.begin() + 1;
		while ( it != pa_src.end() ) {
			if ( x_min > it->x ) x_min = it->x;
			if ( y_min > it->y ) y_min = it->y;
			if ( x_max < it->x ) x_max = it->x;
			if ( y_max < it->y ) y_max = it->y;
			it++;
		}
		rp::rect rect( x_min, y_min, x_max, y_max );
		if ( global ) rect.transform( globalMatrix() );
		bound_rect = rect;
	}
	return bound_rect;
}

void RPShape::drawPolyline( CDC& dc )
{
	if ( pa_global.size() < 2 ) return;
	dc.SelectObject( main_pen );
	dc.BeginPath();
	dc.Polyline( &pa_global[0], pa_global.size() );
	dc.EndPath();
	dc.StrokePath();
}

/*
void RPShape::drawConnectorsInput( CDC& dc )
{
	if ( flowChart->getShowConnectorPoint() ) {
		painter.setPen( flowChart->getShapeColor() );
		painter.setBrush( flowChart->getConnectorDockColor() );
		for ( unsigned int i = 0; i < conI.size(); i++ ) {
			painter.drawEllipse( conI.point(i).x() - 3, conI.point(i).y() - 3, 7, 7 );
		}
	}
}

void RPShape::drawConnectorsOutput( CDC& dc )
{
	if ( flowChart->getShowConnectorPoint() ) {
		painter.setPen( flowChart->getShapeColor() );
		painter.setBrush( flowChart->getConnectorDockColor() );
		for ( unsigned int i = 0; i < conO.size(); i++ ) {
			painter.drawEllipse( conO.point(i).x() - 3, conO.point(i).y() - 3, 7, 7 );
		}
	}
}
*/

void RPShape::draw( CDC& dc )
{
	drawPolyline( dc );

/*
	if ( flowChart->getShowShapeName() ) {
		painter.setPen( flowChart->getShapeColor() );
		int name_x = getX() + flowChart->getPaperBorder();
		int name_y = getY() + flowChart->getPaperBorder();
		painter.drawText( name_x, name_y, size.width(), size.height()-1, Qt::AlignHCenter | Qt::AlignVCenter | Qt::SingleLine, name );
	}
	drawConnectorsInput( painter );
	drawConnectorsOutput( painter );
*/
}


RPChartObject::PossibleCommand RPShape::getPossibleCommand( int global_x, int global_y ) const
{
	return RPChartObject::pcmd_move;
}
