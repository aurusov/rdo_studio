/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_rule.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      18.08.2010
  \brief     Продукционные правила
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RULE_H_
#define _LIB_RUNTIME_RULE_H_

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

/*!
  \class     RDORule
  \brief     Продукционное правило
*/
class RDORule: public IBaseOperation, public IRule, public RDOActivityPattern<RDOPatternRule>, public RDOPatternPrior
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
	RDORule(CREF(LPRDORuntime) pRuntime, CREF(LPRDOPatternRule) pPattern, rbool trace, CREF(tstring) name);
	RDORule(CREF(LPRDORuntime) pRuntime, CREF(LPRDOPatternRule) pPattern, rbool trace, CREF(LPRDOCalc) pCondition, CREF(tstring) name);

	virtual ~RDORule();

	void init();

	LPRDORuntime m_pRuntime;
	LPRDOCalc    m_pAdditionalCondition;
	rbool        m_traceOFF;

	void trace();

	DECLARE_IBaseOperation;
	DECLARE_IRule;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RULE_H_
