#ifndef RDO_OPERATION_H
#define RDO_OPERATION_H

#include "rdo.h"
#include "rdotrace.h"
#include "simtrace.h"
#include "rdo_pattern.h"
#include "rdo_activity.h"
#include "rdo_priority.h"
#include "rdo_operation_interface.h"

namespace rdoRuntime
{

// ----------------------------------------------------------------------------
// ---------- RDOOperation
// ----------------------------------------------------------------------------
class RDOOperation: public IBaseOperation, public IOperation, public RDOActivityPattern<RDOPatternOperation>, public RDOPatternPrior
{
typedef RDOActivityPattern<RDOPatternOperation> pattern_type;
RDO_IOBJECT(RDOOperation);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(pattern_type)
	QUERY_INTERFACE_PARENT(RDOPatternPrior)
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IOperation)
QUERY_INTERFACE_END

friend class RDOTrace;
friend class RDOSimulatorTrace;
friend class RDOSimulator;

public:
	RDOOperation( RDORuntime* runtime, RDOPatternOperation* pattern, bool trace, const std::string& name );
	RDOOperation( RDORuntime* runtime, RDOPatternOperation* pattern, bool trace, RDOCalc* condition, const std::string& name );
	virtual ~RDOOperation();

protected:
	DECLARE_IOperation;

private:
	RDOOperation(PTR(RDORuntime) runtime, CREF(RDOOperation) originForClone);

	typedef std::vector<LPIOperation> CloneList;
	CloneList m_cloneList;

	bool     haveAdditionalCondition;
	RDOCalc* additionalCondition;

	std::string traceOperId() { return rdo::toString(m_operId); }

	int m_operId;

	DECLARE_IBaseOperation;
};

} // namespace rdoRuntime

#endif // RDO_OPERATION_H
