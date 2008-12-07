#ifndef RDO_PROCESS_SHAPE_IF_H
#define RDO_PROCESS_SHAPE_IF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_shape.h"

// ----------------------------------------------------------------------------
// ---------- RDOPROCShapeIf
// ----------------------------------------------------------------------------
class RDOPROCShapeIf: public RDOPROCShape
{
public:
	RDOPROCShapeIf( RDOPROCFlowChart* flowchart, RDOPROCObject* parent );
	virtual ~RDOPROCShapeIf();
};

#endif // RDO_PROCESS_SHAPE_IF_H
