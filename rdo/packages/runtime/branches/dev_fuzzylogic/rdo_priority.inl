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
#include "utils/rdomacros.h"
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

inline rbool RDOPatternPrior::setPrior(CREF(LPRDOCalc) pPrior)
{
	m_pPrior = pPrior;
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDODPTActivityCompare
// --------------------------------------------------------------------------------
inline RDODPTActivityCompare::RDODPTActivityCompare(CREF(LPRDORuntime) pRuntime)
	: m_pRuntime(pRuntime)
{}

inline rbool RDODPTActivityCompare::operator() (CREF(LPIBaseOperation) opr1, CREF(LPIBaseOperation) opr2)
{
	LPIPriority pattern1 = opr1;
	LPIPriority pattern2 = opr2;
	if (pattern1 && pattern2)
	{
		LPRDOCalc prior1 = pattern1->getPrior();
		LPRDOCalc prior2 = pattern2->getPrior();
		RDOValue value1 = prior1 ? prior1->calcValue(m_pRuntime) : RDOValue(0.0);
		RDOValue value2 = prior2 ? prior2->calcValue(m_pRuntime) : RDOValue(0.0);
		return value1 > value2;
	}
	return false;
}

CLOSE_RDO_RUNTIME_NAMESPACE
