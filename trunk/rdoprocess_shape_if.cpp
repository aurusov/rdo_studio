#include "stdafx.h"
#include "rdoprocess_shape_if.h"
#include "rdoprocess_flowchart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPShapeIf
// ----------------------------------------------------------------------------
RPShapeIf::RPShapeIf( RPObject* _parent, RPFlowChart* _flowchart ):
	RPShape( _parent, _flowchart, _T("If") )
{
	pa_src.push_back( CPoint(0, -35) );
	pa_src.push_back( CPoint(35, 0) );
	pa_src.push_back( CPoint(0, 35) );
	pa_src.push_back( CPoint(-35, 0) );
	pa_src.push_back( CPoint(0, -35) );
//	snap_to_point.x = getSize().cx / 2;
//	snap_to_point.y = getSize().cy / 2;
}

RPShapeIf::~RPShapeIf()
{
}
