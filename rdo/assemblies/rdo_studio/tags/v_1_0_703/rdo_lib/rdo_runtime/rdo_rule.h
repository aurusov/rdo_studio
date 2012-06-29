#ifndef RDO_RULE_H
#define RDO_RULE_H

#include "rdo.h"
#include "rdotrace.h"
#include "rdo_pattern.h"
#include "rdo_activity.h"

namespace rdoRuntime {

// ----------------------------------------------------------------------------
// ---------- RDORule
// ----------------------------------------------------------------------------
class RDORule: public RDOActivityPattern<RDOPatternRule>, public RDORuntimeContainer
{
friend class RDOTrace;
friend class RDODPTSearch;
friend class TreeNode;

public:
	RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, const std::string& name );
	RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, RDOCalc* condition, const std::string& name );

	virtual ~RDORule() {}

private:
	void init();
	virtual bool     onCheckCondition( RDOSimulator* sim );
	virtual BOResult onDoOperation   ( RDOSimulator* sim );

	RDOCalc* m_additionalCondition;

	bool m_traceOFF;
	void trace();

	void onBeforeChoiceFrom( RDOSimulator* sim   )    { setPatternParameters(sim); }
	bool choiceFrom        ( RDORuntime* runtime );
	void onBeforeRule      ( RDOSimulator* sim   )    {                            }
	void convertRule       ( RDORuntime* runtime );
	void onAfterRule       ( RDOSimulator* sim, bool inSearch = false );
};

} // namespace rdoRuntime

#endif // RDO_RULE_H
