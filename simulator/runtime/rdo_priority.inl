/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_priority.inl
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      03.10.2009
  \brief     Описание приоритета активностей. Используется при сортировке БЗ.
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdomacros.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOPatternPrior
// --------------------------------------------------------------------------------
inline RDOPatternPrior::RDOPatternPrior()
{}

inline RDOPatternPrior::~RDOPatternPrior()
{}

inline LPRDOCalc RDOPatternPrior::getPrior()
{
	return m_pPrior; 
}

inline bool RDOPatternPrior::setPrior(const LPRDOCalc& pPrior)
{
	m_pPrior = pPrior;
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityCompare
// --------------------------------------------------------------------------------
inline RDODPTActivityCompare::RDODPTActivityCompare(const LPRDORuntime& pRuntime)
	: m_pRuntime(pRuntime)
{}

inline bool RDODPTActivityCompare::operator()(const LPIBaseOperation& pOpr1, const LPIBaseOperation& pOpr2)
{
	LPIPriority pPattern1 = pOpr1;
	LPIPriority pPattern2 = pOpr2;
	if (pPattern1 && pPattern2)
	{
		LPRDOCalc pPrior1 = pPattern1->getPrior();
		LPRDOCalc pPrior2 = pPattern2->getPrior();
		RDOValue value1 = pPrior1 ? pPrior1->calcValue(m_pRuntime) : RDOValue(0.0);
		RDOValue value2 = pPrior2 ? pPrior2->calcValue(m_pRuntime) : RDOValue(0.0);
		return value1 > value2;
	}
	return false;
}

CLOSE_RDO_RUNTIME_NAMESPACE
