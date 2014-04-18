/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_logic.inl
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      31.07.2011
  \brief     Логические операторы
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcAnd
// --------------------------------------------------------------------------------
inline RDOCalcAnd::RDOCalcAnd(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: parent_type(pLeft, pRight)
{
	m_value_true  = 1;
	m_value_false = 0;
}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcOr
// --------------------------------------------------------------------------------
inline RDOCalcOr::RDOCalcOr(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: parent_type(pLeft, pRight)
{
	m_value_true  = 1;
	m_value_false = 0;
}

CLOSE_RDO_RUNTIME_NAMESPACE
