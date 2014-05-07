/*!
  \copyright (c) RDO-Team, 2014
  \file      calc_binary.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      02.05.2014
  \brief     Бинарные операторы
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/operation/calc_binary.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcBinaryBase
// --------------------------------------------------------------------------------
RDOCalcBinaryBase::RDOCalcBinaryBase(const LPRDOCalc& pLeft, const LPRDOCalc& pRight)
	: m_pLeft (pLeft )
	, m_pRight(pRight)
{
	ASSERT(m_pLeft );
	ASSERT(m_pRight);
}

CLOSE_RDO_RUNTIME_NAMESPACE
