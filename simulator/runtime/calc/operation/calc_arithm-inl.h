/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_arithm-inl.h
  \author    Лущан Дмитрий (dluschan@rk9.bmstu.ru)
  \date      26.07.2011
  \brief     Арифметические операции
  \indent    4T
*/

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcDiv
// --------------------------------------------------------------------------------
inline RDOCalcDiv::RDOCalcDiv(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: parent_type(pLeft, pRight)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcPlusEnumSafe
// --------------------------------------------------------------------------------
inline RDOCalcPlusEnumSafe::RDOCalcPlusEnumSafe(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: RDOCalcPlus(pLeft, pRight)
{}

// --------------------------------------------------------------------------------
// -------------------- RDOCalcMultEnumSafe
// --------------------------------------------------------------------------------
inline RDOCalcMultEnumSafe::RDOCalcMultEnumSafe(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: RDOCalcMult(pLeft, pRight)
{}

CLOSE_RDO_RUNTIME_NAMESPACE
