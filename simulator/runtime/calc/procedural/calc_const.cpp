/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_const.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     Пустая операция
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_const.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcConst
// --------------------------------------------------------------------------------
RDOCalcConst::RDOCalcConst(const RDOValue& value)
	: m_value(value)
{
	ASSERT(m_value.typeID() != RDOType::t_unknow);
}

const RDOValue& RDOCalcConst::getValue() const
{
	return m_value;
}

RDOValue RDOCalcConst::doCalc(const LPRDORuntime& pRuntime)
{
	UNUSED(pRuntime);

	return m_value;
}

CLOSE_RDO_RUNTIME_NAMESPACE
