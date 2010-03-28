#ifndef RDO_ACTION_H
#define RDO_ACTION_H

#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/simtrace.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
#include "rdo_lib/rdo_runtime/rdo_action_interface.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOAction
// ----------------------------------------------------------------------------
class RDOAction: public IBaseOperation, public IAction, public RDOActivityPattern<RDOPatternAction>, public RDOPatternPrior, public IActionTrace
{
typedef RDOActivityPattern<RDOPatternAction> pattern_type;
DEFINE_FACTORY(RDOAction);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(pattern_type)
	QUERY_INTERFACE_PARENT(RDOPatternPrior)
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IAction)
	QUERY_INTERFACE(IActionTrace)
QUERY_INTERFACE_END

friend class RDOTrace;
friend class RDOSimulatorTrace;
friend class RDOSimulator;

protected:
	RDOAction( RDORuntime* runtime, RDOPatternAction* pattern, bool trace, const std::string& name );
	RDOAction( RDORuntime* runtime, RDOPatternAction* pattern, bool trace, RDOCalc* condition, const std::string& name );
	virtual ~RDOAction();
	DECLARE_IAction;

private:
	RDOAction(PTR(RDORuntime) runtime, CREF(RDOAction) originForClone);

	typedef std::vector<LPIAction> CloneList;
	CloneList m_cloneList;

	bool     haveAdditionalCondition;
	RDOCalc* additionalCondition;

	int m_actionId;

	DECLARE_IBaseOperation;
	DECLARE_IActionTrace;
};

} // namespace rdoRuntime

#endif // RDO_ACTION_H
