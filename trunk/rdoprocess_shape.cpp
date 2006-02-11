#include "stdafx.h"
#include "rdoprocess_shape.h"
#include "rdoprocess_flowchart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCShape
// ----------------------------------------------------------------------------
RDOPROCShape::RDOPROCShape( RDOPROCObject* _parent, RDOPROCFlowChart* _flowchart ):
	RDOPROCChartObject( _parent, _flowchart->chobj, _flowchart )
//	snap_to_point( 0, 0 )
{
	flowchart->insertShape( this );
}

RDOPROCShape::~RDOPROCShape()
{
	flowchart->deleteShape( this );
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
	dc.SelectObject( main_pen );
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
