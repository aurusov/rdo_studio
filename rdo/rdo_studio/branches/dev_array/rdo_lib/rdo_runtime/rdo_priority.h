/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_priority.h
 * author   : Лущан Дмитрий
 * date     : 04.11.09
 * bref     : Описание приоритета активностей. Используется при сортировке БЗ.
 * indent   : 4T
 */

#ifndef _RDO_PRIORITY_H_
#define _RDO_PRIORITY_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_lib/rdo_runtime/rdo.h"
#include "rdo_lib/rdo_runtime/rdocalc.h"
#include "rdo_lib/rdo_runtime/rdo_runtime.h"
#include "rdo_lib/rdo_runtime/rdo_priority_i.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDOPatternPrior
// ----------------------------------------------------------------------------
class RDOPatternPrior: public IPriority
{
QUERY_INTERFACE_BEGIN
	QUERY_INTERFACE(IPriority)
QUERY_INTERFACE_END

protected:
	RDOPatternPrior()
	{}
	virtual ~RDOPatternPrior()
	{}

private:
	virtual LPRDOCalc getPrior()
	{
		return m_pPrior; 
	}
	virtual rbool setPrior(CREF(LPRDOCalc) pPrior)
	{
		m_pPrior = pPrior;
		return true;
	}

	LPRDOCalc m_pPrior;
};

// ----------------------------------------------------------------------------
// ---------- RDODPTActivityCompare
// ----------------------------------------------------------------------------
class RDODPTActivityCompare
{
public:
	RDODPTActivityCompare(PTR(RDORuntime) runtime)
		: m_runtime(runtime)
	{}
	rbool operator() (CREF(LPIBaseOperation) opr1, CREF(LPIBaseOperation) opr2)
	{
		LPIPriority pattern1 = opr1;
		LPIPriority pattern2 = opr2;
		if (pattern1 && pattern2)
		{
			LPRDOCalc prior1 = pattern1->getPrior();
			LPRDOCalc prior2 = pattern2->getPrior();
			RDOValue value1 = prior1 ? prior1->calcValue(m_runtime) : RDOValue(0.0);
			RDOValue value2 = prior2 ? prior2->calcValue(m_runtime) : RDOValue(0.0);
			return value1 > value2;
		}
		return false;
	}

private:
	PTR(RDORuntime) m_runtime;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdo_priority.inl"

#endif //! _RDO_PRIORITY_H_
