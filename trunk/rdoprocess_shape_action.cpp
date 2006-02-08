#include "stdafx.h"
#include "rdoprocess_shape_action.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOPROCShapeAction
// ----------------------------------------------------------------------------
RDOPROCShapeAction::RDOPROCShapeAction( RDOPROCObject* _parent, RDOPROCFlowChart* _flowchart ):
	RDOPROCShape( _parent, _flowchart )
{
	pa_src.push_back( CPoint(-40, -25) );
	pa_src.push_back( CPoint(40, -25) );
	pa_src.push_back( CPoint(40, 25) );
	pa_src.push_back( CPoint(-40, 25) );
	pa_src.push_back( CPoint(-40, -25) );
	setName( _T("Action") );

	matrix_scale.data[0][0] = 2;
}

RDOPROCShapeAction::~RDOPROCShapeAction()
{
}
