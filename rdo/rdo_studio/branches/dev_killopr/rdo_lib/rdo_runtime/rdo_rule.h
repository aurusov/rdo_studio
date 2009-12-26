#ifndef RDO_RULE_H
#define RDO_RULE_H

#include "rdo.h"
#include "rdotrace.h"
#include "rdo_pattern.h"
#include "rdo_activity.h"
#include "rdo_priority.h"
#include "rdo_rule_interface.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDORule
// ----------------------------------------------------------------------------
class RDORule: public IBaseOperation, public IRule, public RDOActivityPattern<RDOPatternRule>, public RDOPatternPrior, public RDORuntimeContainer
{
typedef RDOActivityPattern<RDOPatternRule> pattern_type;
DEFINE_FACTORY(RDORule);
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE_PARENT(pattern_type)
	QUERY_INTERFACE_PARENT(RDOPatternPrior)
	QUERY_INTERFACE(IBaseOperation)
	QUERY_INTERFACE(IRule)
QUERY_INTERFACE_END

friend class RDOTrace;
friend class RDODPTSearch;
friend class TreeNode;

private:
	RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, const std::string& name );
	RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, RDOCalc* condition, const std::string& name );

	virtual ~RDORule()
	{}

	void init();

	RDOCalc* m_additionalCondition;

	bool m_traceOFF;
	void trace();

	DECLARE_IBaseOperation;
	DECLARE_IRule;
};

} // namespace rdoRuntime

#endif // RDO_RULE_H
