#include "stdafx.h"
#include "rdoprocess_shape.h"
#include "rdoprocess_flowchart.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCShape
// ----------------------------------------------------------------------------
RDOPROCShape::RDOPROCShape( RDOPROCObject* _parent, RDOPROCFlowChart* _flowchart ):
	RDOPROCChartObject( _parent, _flowchart->chobj, _flowchart ),
//	x( 0 ),
//	y( 0 ),
	bound_rect( 0, 0, 0, 0 ),
	snap_to_point( 0, 0 )
{
	flowchart->insertShape( this );
}

RDOPROCShape::~RDOPROCShape()
{
	flowchart->deleteShape( this );
}

void RDOPROCShape::transformToGlobal()
{
	RDOPROCChartObject::transformToGlobal();
//	conI.translate( dx, dy );
//	conO.translate( dx, dy );
//	snap_to_point.Offset( dx, dy );
}

CRect RDOPROCShape::getBoundingRect()
{
	if ( bound_rect.IsRectNull() ) {
		if ( !pa_global.empty() ) {
			int x_min = pa_global[0].x;
			int y_min = pa_global[0].y;
			int x_max = pa_global[0].x;
			int y_max = pa_global[0].y;
			std::vector< CPoint >::const_iterator it = pa_global.begin() + 1;
			while ( it != pa_global.end() ) {
				if ( x_min > it->x ) x_min = it->x;
				if ( y_min > it->y ) y_min = it->y;
				if ( x_max < it->x ) x_max = it->x;
				if ( y_max < it->y ) y_max = it->y;
				it++;
			}
			bound_rect.SetRect( x_min, y_min, x_max, y_max );
		} else {
			bound_rect.SetRect( 0, 0, 0, 0 );
		}
	}

	CRect rect( bound_rect );
	rect.OffsetRect( matrix_transform.dx(), matrix_transform.dy() );
	rect.bottom += flowchart->getPenShapeWidth();
	rect.right  += flowchart->getPenShapeWidth();
	return rect;
}

void RDOPROCShape::setSelected( bool value )
{
	bool _selected = selected;
	RDOPROCObject::setSelected( value );
	if ( flowchart && _selected != value ) {
		flowchart->selectShape( this, value );
	}
}

void RDOPROCShape::setPosition( int x, int y )
{
	RDOPROCChartObject::setPosition( x, y );
	flowchart->snapToGrid( this );
	flowchart->modify();
}

void RDOPROCShape::drawPolyline( CDC& dc )
{
	if ( pa_global.size() < 2 ) return;
	dc.BeginPath();
	dc.Polyline( &pa_global[0], pa_global.size() );
	dc.EndPath();
	dc.StrokePath();
}

/*
void RDOPROCShape::drawConnectorsInput( CDC& dc )
{
	if ( flowChart->getShowConnectorPoint() ) {
		painter.setPen( flowChart->getShapeColor() );
		painter.setBrush( flowChart->getConnectorDockColor() );
		for ( unsigned int i = 0; i < conI.size(); i++ ) {
			painter.drawEllipse( conI.point(i).x() - 3, conI.point(i).y() - 3, 7, 7 );
		}
	}
}

void RDOPROCShape::drawConnectorsOutput( CDC& dc )
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

void RDOPROCShape::draw( CDC& dc )
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
