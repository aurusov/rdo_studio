#ifndef RDO_PROCESS_PROJECT_H
#define RDO_PROCESS_PROJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"

// ----------------------------------------------------------------------------
// ---------- RDOPROCProject
// ----------------------------------------------------------------------------
class RDOPROCProject: public RDOPROCObject
{
public:
	enum FlowState { flow_select, flow_connector, flow_rotate };

protected:
	FlowState flow_state;

public:
	RDOPROCProject();
	virtual ~RDOPROCProject();

	FlowState getFlowState() const { return flow_state; }
	void setFlowState( FlowState value );
};

#endif // RDO_PROCESS_CHART_OBJECT_H
