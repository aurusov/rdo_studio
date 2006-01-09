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
RDOPROCShapeAction::RDOPROCShapeAction( RDOPROCFlowChart* _flowchart, RDOPROCObject* parent ):
	RDOPROCShape( _flowchart, parent )
{
	pa.push_back( CPoint(0, 0) );
	pa.push_back( CPoint(80, 0) );
	pa.push_back( CPoint(80, 50) );
	pa.push_back( CPoint(0, 50) );
	pa.push_back( CPoint(0, 0) );
	setName( _T("Action") );
}

RDOPROCShapeAction::~RDOPROCShapeAction()
{
}
