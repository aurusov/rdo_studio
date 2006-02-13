#ifndef RDO_PROCESS_PROJECT_H
#define RDO_PROCESS_PROJECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"

// ----------------------------------------------------------------------------
// ---------- RPProject
// ----------------------------------------------------------------------------
class RPProject: public RPObject
{
public:
	enum FlowState { flow_select = 0, flow_connector, flow_rotate };

protected:
	FlowState flow_state;
	virtual void notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam );

public:
	RPProject();
	virtual ~RPProject();

	FlowState getFlowState() const { return flow_state; }
	void setFlowState( FlowState value );
};

#endif // RDO_PROCESS_CHART_OBJECT_H
