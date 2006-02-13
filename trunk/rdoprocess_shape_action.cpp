#include "stdafx.h"
#include "rdoprocess_shape_action.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPShapeAction
// ----------------------------------------------------------------------------
RPShapeAction::RPShapeAction( RPObject* _parent, RPFlowChart* _flowchart ):
	RPShape( _parent, _flowchart, _T("Action") )
{
	pa_src.push_back( CPoint(-40, -25) );
	pa_src.push_back( CPoint(40, -25) );
	pa_src.push_back( CPoint(40, 25) );
	pa_src.push_back( CPoint(-40, 25) );
	pa_src.push_back( CPoint(-40, -25) );

	matrix_scale.data[0][0] = 2;
}

RPShapeAction::~RPShapeAction()
{
}
