/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_nop.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      17.12.2011
  \brief     Пустая операция
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/procedural/calc_nop.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOCalcNop
// --------------------------------------------------------------------------------
RDOCalcNop::RDOCalcNop()
{}

RDOCalcNop::~RDOCalcNop()
{}

RDOValue RDOCalcNop::doCalc(CREF(LPRDORuntime) pRuntime)
{
	UNUSED(pRuntime);
	return RDOValue();
}

CLOSE_RDO_RUNTIME_NAMESPACE
