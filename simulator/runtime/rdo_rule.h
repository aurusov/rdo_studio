#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo.h"
#include "simulator/runtime/rdotrace.h"
#include "simulator/runtime/rdo_pattern.h"
#include "simulator/runtime/rdo_activity.h"
#include "simulator/runtime/rdo_priority.h"
#include "simulator/runtime/rdo_rule_i.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDORule: public IBaseOperation, public IRule, public RDOActivityPattern<RDOPatternRule>, public RDOPatternPrior
{
typedef RDOActivityPattern<RDOPatternRule> pattern_type;
DECLARE_FACTORY(RDORule);
friend class RDOTrace;
friend class RDODPTSearch;
friend class TreeNode;

private:
	RDORule(const LPRDORuntime& pRuntime, const LPRDOPatternRule& pPattern, bool trace, const std::string& name);
	RDORule(const LPRDORuntime& pRuntime, const LPRDOPatternRule& pPattern, bool trace, const LPRDOCalc& pCondition, const std::string& name);

	virtual ~RDORule();

	void init();

	LPRDORuntime m_pRuntime;
	LPRDOCalc m_pAdditionalCondition;
	bool m_traceOFF;

	void trace();

	DECLARE_IBaseOperation;
	DECLARE_IRule;
};

CLOSE_RDO_RUNTIME_NAMESPACE
