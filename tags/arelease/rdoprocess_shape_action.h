#ifndef RDO_PROCESS_SHAPE_ACTION_H
#define RDO_PROCESS_SHAPE_ACTION_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_shape.h"

// ----------------------------------------------------------------------------
// ---------- RDOPROCShapeAction
// ----------------------------------------------------------------------------
class RDOPROCShapeAction: public RDOPROCShape
{
public:
	RDOPROCShapeAction( RDOPROCFlowChart* flowchart, RDOPROCObject* parent );
	virtual ~RDOPROCShapeAction();
};

#endif // RDO_PROCESS_SHAPE_ACTION_H
