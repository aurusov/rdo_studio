#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_dptsearch_activity_i.h"
// --------------------------------------------------------------------------------

class IDPTSearchLogic: public virtual rdo::counter_reference
{
public:
	virtual void addActivity(LPIDPTSearchActivity activity) = 0;
};
DECLARE_POINTER(IDPTSearchLogic)

#define DECLARE_IDPTSearchLogic \
	virtual void addActivity(LPIDPTSearchActivity activity);
