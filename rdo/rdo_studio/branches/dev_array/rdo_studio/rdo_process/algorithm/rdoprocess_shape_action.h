#ifndef RDO_PROCESS_SHAPE_ACTION_H
#define RDO_PROCESS_SHAPE_ACTION_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "rdo_studio/rdo_process/rp_method/rdoprocess_shape.h"
// ----------------------------------------------------------------------------
// ---------- RPShapeAction
// ----------------------------------------------------------------------------
class RPShapeAction: public RPShape
{
friend class RPMethodAlgorithm;
private:
	static RPObject* newObject( RPObject* parent );
public:
	RPShapeAction( RPObject* parent );
	virtual ~RPShapeAction();
	virtual rp::string getClassName() const { return "RPShapeAction"; }
};

#endif // RDO_PROCESS_SHAPE_ACTION_H
