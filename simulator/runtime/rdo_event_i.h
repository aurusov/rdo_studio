#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace runtime {

PREDECLARE_POINTER(RDORuntime)

}} // namespace rdo::runtime

class IEvent: public virtual rdo::counter_reference
{
public:
	virtual void onMakePlaned(const rdo::runtime::LPRDORuntime& pRuntime, const std::vector<rdo::runtime::RDOValue>& params) = 0;

	virtual ~IEvent()
	{}
};
DECLARE_POINTER(IEvent);
