#include "stdafx.h"
#include "rdoprocess_chart_object.h"
#include "rdoprocess_flowchart.h"

#include <math.h>

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
	chart_parent( _chart_parent ),
	flowchart( _flowchart )
{
}

RDOPROCChartObject::~RDOPROCChartObject()
{
}

RDOPROCMatrix RDOPROCChartObject::globalMatrix() const
{
	return chart_parent ? chart_parent->globalMatrix() * matrix_transform * matrix_rotate * matrix_scale : matrix_transform * matrix_rotate * matrix_scale;
}

RDOPROCMatrix RDOPROCChartObject::parentMatrix() const
{
	return chart_parent ? chart_parent->matrix_transform : RDOPROCMatrix();
}

void RDOPROCChartObject::transformToGlobal()
{
	if ( pa_global.size() != pa_src.size() ) {
		pa_global.resize( pa_src.size() );
	}
	trans tr( globalMatrix() );
	std::transform( pa_src.begin(), pa_src.end(), pa_global.begin(), tr );
}

bool RDOPROCChartObject::pointInPolygon( int x, int y ) const
{
	if ( pa_global.size() > 2 ) {
		unsigned int i;
		unsigned int j;
		int k0 = 0;
		bool flag = true;
		for ( i = 0, j = 1; i < pa_global.size()-1; i++, j++ ) {
			const CPoint& p1 = pa_global[i];
			const CPoint& p2 = pa_global[j];
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

void RDOPROCChartObject::moving( int dx, int dy )
{
	matrix_transform.dx() += dx;
	matrix_transform.dy() += dy;
	flowchart->modify();
}

void RDOPROCChartObject::setRotation( double alpha )
{
	const double pi = 3.14159265358979323846;
	alpha = alpha * pi / 180.0;
	double cosa = cos( alpha );
	double sina = sin( alpha );
//	if ( fabs(cosa) < 1e-10 ) cosa = 0;
//	if ( fabs(sina) < 1e-10 ) sina = 0;
	matrix_rotate.data[0][0] = cosa;
	matrix_rotate.data[1][1] = cosa;
	matrix_rotate.data[0][1] = -sina;
	matrix_rotate.data[1][0] = sina;
}
