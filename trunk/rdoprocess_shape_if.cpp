#include "stdafx.h"
#include "rdoprocess_shape_if.h"
#include "rdoprocess_flowchart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCShapeIf
// ----------------------------------------------------------------------------
RDOPROCShapeIf::RDOPROCShapeIf( RDOPROCFlowChart* _flowchart, RDOPROCObject* parent ):
	RDOPROCShape( _flowchart, parent )
{
	pa.push_back( CPoint(35, 0) );
	pa.push_back( CPoint(70, 35) );
	pa.push_back( CPoint(35, 70) );
	pa.push_back( CPoint(0, 35) );
	pa.push_back( CPoint(35, 0) );
	snap_to_point.x = getSize().cx / 2 - flowchart->getPenShapeWidth() / 2;
	snap_to_point.y = getSize().cy / 2 - flowchart->getPenShapeWidth() / 2;
	setName( _T("If") );
}

RDOPROCShapeIf::~RDOPROCShapeIf()
{
}
