/*
 * copyright: (c) RDO-Team, 2010
 * filename : rdo_rule.h
 * author   : Урусов Андрей, Лущан Дмитрий
 * date     : 18.08.2010
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_RULE_H_
#define _RDO_RULE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdotrace.h"
#include "rdo_lib/rdo_runtime/rdo_pattern.h"
#include "rdo_lib/rdo_runtime/rdo_activity.h"
#include "rdo_lib/rdo_runtime/rdo_priority.h"
#include "rdo_lib/rdo_runtime/rdo_rule_interface.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORule
// ----------------------------------------------------------------------------
class RDORule: public IBaseOperation, public IRule, public RDOActivityPattern<RDOPatternRule>, public RDOPatternPrior, public RDORuntimeContainer
{
typedef RDOActivityPattern<RDOPatternRule> pattern_type;
DEFINE_IFACTORY(RDORule);
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
	RDORule( RDORuntime* runtime, RDOPatternRule* pattern, bool trace, CREF(LPRDOCalc) pCondition, const std::string& name );

	virtual ~RDORule()
	{}

	void init();

	LPRDOCalc m_additionalCondition;

	bool m_traceOFF;
	void trace();

	DECLARE_IBaseOperation;
	DECLARE_IRule;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif //! _RDO_RULE_H_
