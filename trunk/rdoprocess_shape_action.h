#ifndef RDO_PROCESS_SHAPE_ACTION_H
#define RDO_PROCESS_SHAPE_ACTION_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_shape.h"

// ----------------------------------------------------------------------------
// ---------- RPShapeAction
// ----------------------------------------------------------------------------
class RPShapeAction: public RPShape
{
public:
	RPShapeAction( RPObject* parent, RPFlowChart* flowchart );
	virtual ~RPShapeAction();
};

#endif // RDO_PROCESS_SHAPE_ACTION_H
