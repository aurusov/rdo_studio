#ifndef RDO_PROCESS_SHAPE_IF_H
#define RDO_PROCESS_SHAPE_IF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdo_studio/rdo_process/rp_method/rdoprocess_shape.h"

// ----------------------------------------------------------------------------
// ---------- RPShapeIf
// ----------------------------------------------------------------------------
class RPShapeIf: public RPShape
{
friend class RPMethodAlgorithm;
private:
	static RPObject* newObject( RPObject* parent );
public:
	RPShapeIf( RPObject* parent );
	virtual ~RPShapeIf();
	virtual rp::string getClassName() const { return "RPShapeIf"; }
};

#endif // RDO_PROCESS_SHAPE_IF_H
