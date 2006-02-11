#include "stdafx.h"
#include "rdoprocess_chart_object.h"
#include "rdoprocess_flowchart.h"
#include "rdoprocess_math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCChartObject
// ----------------------------------------------------------------------------
RDOPROCChartObject::RDOPROCChartObject( RDOPROCObject* _parent, RDOPROCChartObject* _chart_parent, RDOPROCFlowChart* _flowchart ):
	RDOPROCObject( _parent ),
	rotate_center( 0, 0 ),
	rotate_center_inited( false ),
	chart_parent( _chart_parent ),
	flowchart( _flowchart ),
	main_pen_width( 2 )
{
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0x00, 0x00, 0x00);
	lb.lbHatch = 0;
	main_pen.CreatePen( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE | PS_JOIN_MITER, main_pen_width, &lb );
}

RDOPROCChartObject::~RDOPROCChartObject()
{
}

RDOPROCMatrix RDOPROCChartObject::globalMatrix() const
{
	RDOPROCMatrix r_center;
	r_center.dx() = rotate_center.x;
	r_center.dy() = rotate_center.y;
	return chart_parent ? chart_parent->globalMatrix() * matrix_transform * r_center * matrix_rotate * r_center.obr() * matrix_scale : matrix_transform * r_center * matrix_rotate * r_center.obr() * matrix_scale;
}

RDOPROCMatrix RDOPROCChartObject::parentMatrix() const
{
	return chart_parent ? chart_parent->matrix_transform : RDOPROCMatrix();
}

void RDOPROCChartObject::meshToGlobal()
{
	if ( pa_global.size() != pa_src.size() ) {
		pa_global.resize( pa_src.size() );
	}
	trans tr( globalMatrix() );
	std::transform( pa_src.begin(), pa_src.end(), pa_global.begin(), tr );
}

void RDOPROCChartObject::transformToGlobal()
{
	meshToGlobal();
}

void RDOPROCChartObject::moving( int dx, int dy )
{
	matrix_transform.dx() += dx;
	matrix_transform.dy() += dy;
	flowchart->modify();
}

void RDOPROCChartObject::setRotation( double alpha )
{
	alpha = alpha * rp::math::pi / 180.0;
	double cosa = cos( alpha );
	double sina = sin( alpha );
//	if ( fabs(cosa) < 1e-10 ) cosa = 0;
//	if ( fabs(sina) < 1e-10 ) sina = 0;
	matrix_rotate.data[0][0] = cosa;
	matrix_rotate.data[1][1] = cosa;
	matrix_rotate.data[0][1] = -sina;
	matrix_rotate.data[1][0] = sina;
}

rp::RPRect RDOPROCChartObject::getBoundingRect( bool global ) const
{
	rp::RPRect bound_rect;
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
		rp::RPRect rect( x_min, y_min, x_max, y_max );
		if ( global ) rect.transform( globalMatrix() );
		bound_rect = rect;
	}
	return bound_rect;
}
