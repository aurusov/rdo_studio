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
RDOPROCShape::RDOPROCShape( RDOPROCFlowChart* _flowchart, RDOPROCObject* parent ):
	RDOPROCObject( parent ),
	flowchart( _flowchart ),
	x( 0 ),
	y( 0 ),
	bound_rect( 0, 0, 0, 0 ),
	snap_to_point( 0, 0 )
{
	flowchart->insertShape( this );
}

RDOPROCShape::~RDOPROCShape()
{
	flowchart->deleteShape( this );
}

void RDOPROCShape::translate( const int dx, const int dy )
{
	trans tr( dx, dy );
	std::transform( pa.begin(), pa.end(), pa.begin(), tr );
//	conI.translate( dx, dy );
//	conO.translate( dx, dy );
	snap_to_point.Offset( dx, dy );
}

bool RDOPROCShape::pointInPolygon( const int x, const int y ) const
{
	if ( pa.size() > 2 ) {
		unsigned int i;
		unsigned int j;
		int k0 = 0;
		bool flag = true;
		for ( i = 0, j = 1; i < pa.size()-1; i++, j++ ) {
			const CPoint& p1 = pa[i];
			const CPoint& p2 = pa[j];
			int k = (y - p1.y)*(p2.x - p1.x) - (x - p1.x)*(p2.y - p1.y);
			if ( k == 0 ) {
				break;
			} else {
				if ( k0 == 0 ) {
					k0 = k;
				} else {
					if ( k0 * k < 0 ) {
						flag = false;
						break;
					}
				}
			}
		}
		return flag;
	}
	return false;
}

void RDOPROCShape::setSelected( const bool value )
{
	bool _selected = selected;
	RDOPROCObject::setSelected( value );
	if ( flowchart && _selected != value ) {
		flowchart->selectShape( this, value );
	}
}

void RDOPROCShape::move( const int _x, const int _y )
{
	x = _x;
	y = _y;
	if ( x < 0 ) {
		x = 0;
	}
	if ( y < 0 ) {
		y = 0;
	}
}

void RDOPROCShape::setX( const int value )
{
	moveTo( value, getY() );
}

void RDOPROCShape::setY( const int value )
{
	moveTo( getX(), value );
}

void RDOPROCShape::moveTo( const int _x, const int _y )
{
	move( _x, _y );
	flowchart->snapToGrid( this );
	flowchart->modify();
}

CRect RDOPROCShape::getBoundingRect()
{
	if ( bound_rect.IsRectNull() ) {
		if ( !pa.empty() ) {
			int x_min = pa[0].x;
			int y_min = pa[0].y;
			int x_max = pa[0].x;
			int y_max = pa[0].y;
			std::vector< CPoint >::const_iterator it = pa.begin() + 1;
			while ( it != pa.end() ) {
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
	rect.OffsetRect( getX(), getY() );
	return rect;
}

void RDOPROCShape::moving( const int dx, const int dy )
{
	if ( x + dx < 0 ) {
		x = 0;
	} else {
		x += dx;
	}
	if ( y + dy < 0 ) {
		y = 0;
	} else {
		y += dy;
	}
//	update_grid_pos = true;
	flowchart->modify();
}

/*
void RDOPROCShape::movingGrid( const int dx, const int dy )
{
	x += dx * flowchart->getGridStep();
	y += dy * flowchart->getGridStep();
	update_grid_pos = true;
}
*/

int RDOPROCShape::getMovingStopX( const int dx ) const
{
	int delta = x + dx;
	if ( delta < 0 ) {
		return -delta;
	}
	return 0;
}

int RDOPROCShape::getMovingStopY( const int dy ) const
{
	int delta = y + dy;
	if ( delta < 0 ) {
		return -delta;
	}
	return 0;
}

void RDOPROCShape::drawPolyline( CDC& dc )
{
	if ( pa.size() < 2 ) return;
	dc.BeginPath();
	dc.Polyline( &pa[0], pa.size() );
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

	CPen pen_red( PS_SOLID, 1, RGB(-1,0,0) );
	CBrush brush_white( RGB(-1,-1,-1) );
	dc.SelectObject( pen_red );
	dc.SelectObject( brush_white );
	dc.Ellipse( snap_to_point.x - 2, snap_to_point.y - 2, snap_to_point.x + 2, snap_to_point.y + 2 );

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

	if ( selected ) {
		dc.SelectObject( flowchart->getPenShapeColor() );
		dc.SelectObject( flowchart->getBrushSelectBox() );
		int x = getX() + flowchart->getPaperBorderWidth();
		int y = getY() + flowchart->getPaperBorderHeight();
		int w = getSize().cx + flowchart->getPenShapeWidth() - 1;
		int h = getSize().cy + flowchart->getPenShapeWidth() - 1;
		int box_size   = 7;
		int box_size_2 = 3;
		dc.Rectangle( x - box_size_2, y - box_size_2, x - box_size_2 + box_size, y - box_size_2 + box_size );
		dc.Rectangle( x + w - box_size_2, y - box_size_2, x + w - box_size_2 + box_size, y - box_size_2 + box_size );
		dc.Rectangle( x - box_size_2, y + h - box_size_2, x - box_size_2 + box_size, y + h - box_size_2 + box_size );
		dc.Rectangle( x + w - box_size_2, y + h - box_size_2, x + w - box_size_2 + box_size, y + h - box_size_2 + box_size );
	}
}
